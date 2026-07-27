/**
 * @file
 *
 * FrequencySpectrumFactory implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-14
 */

#include "FrequencySpectrumFactory.hpp"

#ifdef __EMSCRIPTEN__
#include "../../web/EmscriptenExtra.hpp"
#else
#include <cmath>
#endif

#include "../../utility/FFTRealComplex.hpp"
#include "../FrequencySpectrum.hpp"
#include "../Signal.hpp"

namespace echomap
{

std::unique_ptr<FrequencySpectrum> FrequencySpectrumFactory::create_frequency_spectrum(
        const Signal& signal,
        const WindowFunctions::AllFunctions window_function,
        const std::size_t transform_size
)
{
    if (transform_size > signal.get_sample_count())
        throw std::runtime_error(
                std::format(
                        "DFT transform size of {} exceeds {} sample count of {}.",
                        transform_size,
                        signal.get_name(),
                        signal.get_sample_count()
                )
        );

    if (!signal.is_uniformly_sampled())
        throw std::runtime_error(std::format("Attempted to transform variably sampled {}.", signal.get_name()));

    const auto display_name = std::format(
            "{} ({} DFT @ {})",
            signal.get_name(),
            WindowFunctions::indexed_names[window_function.index()],
            transform_size
    );

    if (transform_size == 0)
        return std::unique_ptr<FrequencySpectrum>(new FrequencySpectrum(window_function, display_name));

    const auto samples = signal.amplitudes().first(transform_size);

    FFTRealComplex fft(transform_size);

    const auto fft_result = std::visit(
            variant_helpers::Overloaded{
                    [&fft, samples](const WindowFunctions::Constant&) {
                        return fft.execute(samples);
                    },
                    [&fft, samples](auto window) {
                        return fft.execute(samples, std::move(window));
                    },
            },
            window_function
    );

    // Construct the FrequencySpectrum from the coefficients.
    const auto bin_count = transform_size / 2 + 1;
    auto spectrum = std::unique_ptr<FrequencySpectrum>(new FrequencySpectrum(window_function, display_name));
    spectrum->reserve_bins(bin_count);

    for (std::size_t bin_idx = 0; bin_idx < bin_count; ++bin_idx) {
        const auto is_dc = bin_idx == 0;
        const auto is_nyquist = transform_size % 2 == 0 && bin_idx == transform_size / 2;
        const auto scale = (is_dc || is_nyquist ? 1.0f : 2.0f) / fft_result.scale_divisor;

        const auto real = fft_result.coefficients[bin_idx][0];
        const auto imag = fft_result.coefficients[bin_idx][1];
        const auto linear_amplitude = std::sqrt(real * real + imag * imag) * scale;

        spectrum->emplace_bin(
                static_cast<float>(bin_idx) * static_cast<float>(signal.get_sample_rate()) /
                        static_cast<float>(transform_size),
                amplitude_to_dbfs(linear_amplitude),
                std::atan2(imag, real)
        );
    }

    return spectrum;
}

Signal::Sample::AmplitudeT FrequencySpectrumFactory::amplitude_to_dbfs(
        const Signal::Sample::AmplitudeT amplitude
) noexcept
{
#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)
    constexpr auto full = std::max(web::abs(Signal::normalised_range.first), web::abs(Signal::normalised_range.second));
#else
    constexpr auto full = std::max(std::abs(Signal::normalised_range.first), std::abs(Signal::normalised_range.second));
#endif
    static_assert(full > 0.0f);

    constexpr auto maximum_ratio = 1.0e-6f; // 20log_{10}(1e-6) = -120dB.
    return 20.0f * std::log10(std::max(std::abs(amplitude) / full, maximum_ratio));
}

} // namespace echomap
