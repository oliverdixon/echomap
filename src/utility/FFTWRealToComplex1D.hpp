/**
 * @file
 *
 * FFTWRealToComplex1D specification
 *
 * @author Oliver Dixon
 * @date 2026-07-26
 */

#ifndef ECHOMAP_FFTWREALTOCOMPLEX1D_HPP
#define ECHOMAP_FFTWREALTOCOMPLEX1D_HPP

#include <fftw3.h>

#include <concepts>
#include <functional>
#include <span>

namespace echomap
{

namespace detail
{

template <typename Projection>
concept WindowFunction = std::invocable<Projection&, std::size_t, std::size_t> &&
                         std::convertible_to<std::invoke_result_t<Projection&, std::size_t, std::size_t>, float>;

} // namespace detail

/**
 * @todo Document
 */
class FFTWRealToComplex1D
{
public:
    struct Result
    {
        std::span<const fftwf_complex> coefficients;
        float scale_divisor;
    };

    explicit FFTWRealToComplex1D(
            std::size_t requested_transform_size,
            unsigned int requested_projected_planner_flags = FFTW_ESTIMATE
    );

    ~FFTWRealToComplex1D() noexcept;

    FFTWRealToComplex1D(const FFTWRealToComplex1D&) = delete;
    FFTWRealToComplex1D& operator=(const FFTWRealToComplex1D&) = delete;

    FFTWRealToComplex1D(FFTWRealToComplex1D&& other) noexcept;
    FFTWRealToComplex1D& operator=(FFTWRealToComplex1D&& other) noexcept;

    [[nodiscard]] Result execute(std::span<const float> samples);

    template <detail::WindowFunction Projection>
    [[nodiscard]] Result execute(
            std::span<const float> samples,
            Projection& projection
    )
    {
        validate_input_size(samples);

        auto projected = projected_input_buffer();
        float scale_divisor = 0.0f;

        for (std::size_t i = 0; i < samples.size(); ++i) {
            const auto window_value = projection(i, samples.size());
            scale_divisor += window_value;
            projected[i] = samples[i] * window_value;
        }

        return {
                .coefficients = execute_from_mutable_input(projected.data(), projected_planner_flags),
                .scale_divisor = scale_divisor,
        };
    }

private:
    void validate_input_size(std::span<const float> samples) const;
    [[nodiscard]] std::span<float> projected_input_buffer();

    [[nodiscard]] std::span<const fftwf_complex> execute_from_mutable_input(
            float* input,
            unsigned int planner_flags
    );

    void ensure_plan(
            float* input,
            unsigned int planner_flags
    );
    void destroy_plan() noexcept;
    void reset() noexcept;

    std::size_t transform_size = 0;
    int fftw_transform_size = 0;
    std::size_t output_size = 0;

    unsigned int projected_planner_flags = FFTW_ESTIMATE;

    float* projected_input = nullptr;
    fftwf_complex* output = nullptr;

    fftwf_plan plan = nullptr;
    float* planned_input = nullptr;
    unsigned int planned_flags = 0;
};

} // namespace echomap

#endif // ECHOMAP_FFTWREALTOCOMPLEX1D_HPP
