/**
 * @file
 *
 * WindowFunctions specification
 *
 * @author Oliver Dixon
 * @date 2026-07-16
 */

#ifndef ECHOMAP_WINDOWFUNCTIONS_HPP
#define ECHOMAP_WINDOWFUNCTIONS_HPP

#include <cstddef>
#include <cstdint>
#include <ranges>
#include <string>

namespace echomap
{

/**
 * Aggregation of callable DSP window functions.
 *
 * For some common examples, see https://en.wikipedia.org/wiki/Window_function#Examples_of_window_functions.
 */
class WindowFunctions
{
public:
    /**
     * Selector for pre-processing window functions.
     */
    enum class Function : std::int8_t
    {
        Constant,
        Hann,
        Hamming,
        Bartlett,
        Blackman,
        BlackmanHarris,
        Welch,
    };

    static const char* get_window_function_name_imgui(Function function) noexcept;

    /**
     * The Constant invocable window function.
     */
    struct Constant
    {
        /**
         * Maps the index to the constant 1.0.
         *
         * @param index The index within the window.
         * @param size The size of the window.
         * @return The mapped index.
         */
        static float operator()(
                std::size_t index,
                std::size_t size
        ) noexcept;
    };

    /**
     * The Hann raised-cosine window.
     */
    struct Hann
    {
        /**
         * Maps the index into the Hann function.
         *
         * For a window of size @f$ M @f$, let @f$ N = M - 1 @f$.
         *
         * @f[
         *      w[n] =
         *      \frac{1}{2}
         *      \left(
         *          1 - \cos\left(\frac{2\pi n}{N}\right)
         *      \right)
         * @f]
         *
         * for index @f$ n @f$.
         *
         * @param index The index within the window.
         * @param size The size of the window.
         * @return The Hann-mapped index.
         */
        static float operator()(
                std::size_t index,
                std::size_t size
        ) noexcept;
    };

    /**
     * The Hamming raised-cosine window.
     */
    struct Hamming
    {
        /**
         * Maps the index into the Hamming function.
         *
         * For a window of size @f$ M @f$, let @f$ N = M - 1 @f$.
         *
         * @f[
         *      w[n] =
         *      \alpha
         *      -
         *      \beta\cos\left(\frac{2\pi n}{N}\right)
         * @f]
         *
         * where
         *
         * @f[
         *      \alpha = 0.54,\quad
         *      \beta = 1 - \alpha = 0.46.
         * @f]
         *
         * for index @f$ n @f$.
         *
         * @param index The index within the window.
         * @param size The size of the window.
         * @return The Hamming-mapped index.
         */
        static float operator()(
                std::size_t index,
                std::size_t size
        ) noexcept;
    };

    /**
     * The Bartlett triangular window.
     */
    struct Bartlett
    {
        /**
         * Maps the index into the Bartlett function.
         *
         * For a window of size @f$ M @f$, let @f$ N = M - 1 @f$.
         *
         * @f[
         *      w[n] =
         *      \frac{2}{N}
         *      \left(
         *          \frac{N}{2}
         *          -
         *          \left| n - \frac{N}{2} \right|
         *      \right)
         * @f]
         *
         * for index @f$ n @f$.
         *
         * The endpoints are zero and the centre approaches one. For even window sizes, the exact value one does not
         * occur at an integer index.
         *
         * @param index The index within the window.
         * @param size The size of the window.
         * @return The Bartlett-mapped index.
         */
        static float operator()(
                std::size_t index,
                std::size_t size
        ) noexcept;
    };

    /**
     * The Blackman cosine-sum window.
     */
    struct Blackman
    {
        /**
         * Maps the index into the Blackman function.
         *
         * For a window of size @f$ M @f$, let @f$ N = M - 1 @f$.
         *
         * @f[
         *      w[n] =
         *      0.42
         *      -
         *      0.5\cos\left(\frac{2\pi n}{N}\right)
         *      +
         *      0.08\cos\left(\frac{4\pi n}{N}\right)
         * @f]
         *
         * for index @f$ n @f$.
         *
         * This is the common approximate Blackman window, not the exact sidelobe-nulling Blackman variant.
         *
         * @param index The index within the window.
         * @param size The size of the window.
         * @return The Blackman-mapped index.
         */
        static float operator()(
                std::size_t index,
                std::size_t size
        ) noexcept;
    };

    /**
     * The minimum four-term Blackman-Harris cosine-sum window.
     */
    struct BlackmanHarris
    {
        /**
         * Maps the index into the Blackman-Harris function.
         *
         * For a window of size @f$ M @f$, let @f$ N = M - 1 @f$.
         *
         * @f[
         *      w[n] =
         *      a_0
         *      -
         *      a_1\cos\left(\frac{2\pi n}{N}\right)
         *      +
         *      a_2\cos\left(\frac{4\pi n}{N}\right)
         *      -
         *      a_3\cos\left(\frac{6\pi n}{N}\right)
         * @f]
         *
         * where
         *
         * @f[
         *      a_0 = 0.35875,\quad
         *      a_1 = 0.48829,\quad
         *      a_2 = 0.14128,\quad
         *      a_3 = 0.01168.
         * @f]
         *
         * for index @f$ n @f$.
         *
         * @param index The index within the window.
         * @param size The size of the window.
         * @return The Blackman-Harris-mapped index.
         */
        static float operator()(
                std::size_t index,
                std::size_t size
        ) noexcept;
    };

    /**
     * The Welch parabolic window.
     */
    struct Welch
    {
        /**
         * Maps the index into the Welch function.
         *
         * For a window of size @f$ M @f$, let @f$ N = M - 1 @f$.
         *
         * @f[
         *      w[n] =
         *      1
         *      -
         *      \left(
         *          \frac{n - \frac{N}{2}}{\frac{N}{2}}
         *      \right)^2
         * @f]
         *
         * for index @f$ n @f$.
         *
         * The Welch window is a single parabolic section with zero-valued endpoints and a maximum at the centre.
         *
         * @param index The index within the window.
         * @param size The size of the window.
         * @return The Welch-mapped index.
         */
        static float operator()(
                std::size_t index,
                std::size_t size
        ) noexcept;
    };

    template <class Window>
    static auto make_window(
            const std::size_t size,
            Window window
    )
    {
        return std::views::iota(std::size_t{0}, size) | std::views::transform([size, window](const std::size_t index) {
                   return window(index, size);
               });
    }
};

} // namespace echomap

#endif // ECHOMAP_WINDOWFUNCTIONS_HPP
