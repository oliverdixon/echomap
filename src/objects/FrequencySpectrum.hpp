/**
 * @file
 *
 * FrequencySpectrum specification
 *
 * @author Oliver Dixon
 * @date 2026-07-14
 */

#ifndef ECHOMAP_FREQUENCYSPECTRUM_HPP
#define ECHOMAP_FREQUENCYSPECTRUM_HPP

#include <vector>

#include "Object.hpp"

namespace echomap
{

class FrequencySpectrumFactory;

/**
 * A Signal sampled in the frequency domain.
 */
class FrequencySpectrum : public Object<FrequencySpectrum>
{
public:
    /**
     * Selector for pre-processing window functions.
     */
    enum class WindowFunction
    {
        Identity, /**< Do nothing. */
        Hann,     /**< Hann raised-cosine window. */
        Hamming,  /**< Hamming raised-cosine window. */
    };

    struct Bin
    {
        float frequency;
        float magnitude;
        float phase;
    };

    std::vector<Bin> bins;
    const WindowFunction preprocessor;

    static std::string_view get_window_function_name(WindowFunction function) noexcept;

private:
    friend FrequencySpectrumFactory;

    explicit FrequencySpectrum(
            WindowFunction preprocessor,
            std::string_view name
    );
};

} // namespace echomap

#endif // ECHOMAP_FREQUENCYSPECTRUM_HPP
