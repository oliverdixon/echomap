/**
 * @file
 *
 * FFTWRealComplex specification
 *
 * @author Oliver Dixon
 * @date 2026-07-26
 */

#ifndef ECHOMAP_FFTWREALCOMPLEX_HPP
#define ECHOMAP_FFTWREALCOMPLEX_HPP

#include <fftw3.h>

#include <concepts>
#include <functional>
#include <span>

namespace echomap
{

namespace detail
{

/**
 * Concept for a window function.
 *
 * Models a window function that provides a weighting based on an index and a total transform size.
 *
 * @tparam Candidate The type to test.
 */
template <typename Candidate>
concept WindowFunction = std::invocable<Candidate&, std::size_t, std::size_t> &&
                         std::convertible_to<std::invoke_result_t<Candidate&, std::size_t, std::size_t>, float>;

} // namespace detail

/**
 * RAII wrapper for FFTW3 specialised for one-dimensional real-to-complex DFTs.
 */
class FFTRealComplex
{
public:
    /**
     * Result of a DFT.
     */
    struct Result
    {
        std::span<const fftwf_complex> coefficients; /**< Complex coefficients characterising DFT output. */
        float scale_divisor;                         /**< Accumulation of window function values for scaling output. */
    };

    /**
     * Prepare an FFT plan for a sized real-valued input vector.
     *
     * @param requested_transform_size Size of the real-valued input vector.
     * @param requested_flags Flags for the FFTW planner; these may be overwritten by the implementation.
     */
    explicit FFTRealComplex(
            std::size_t requested_transform_size,
            unsigned int requested_flags = FFTW_ESTIMATE
    );

    /**
     * Destruct internal FFTW state.
     */
    ~FFTRealComplex() noexcept;

    FFTRealComplex(const FFTRealComplex&) = delete;
    FFTRealComplex& operator=(const FFTRealComplex&) = delete;

    FFTRealComplex(FFTRealComplex&& other) noexcept;
    FFTRealComplex& operator=(FFTRealComplex&& other) noexcept;

    /**
     * Compute the DFT on a set of real-valued samples.
     *
     * This overload does not preprocess the input with a window function, hence Result::scale_divisor will be zero.
     * Since the input vector is immutable, requested planner flags are ignored and @c FFTW_ESTIMATE is used.
     *
     * @param samples The transform input.
     * @return The Result describing the DFT complex coefficients and the accumulated window function.
     *
     * @throws std::invalid_argument The size of the transform input does not match the prepared plan.
     */
    [[nodiscard]] Result execute(std::span<const float> samples);

    /**
     * Compute the DFT on a set of real-valued samples.
     *
     * This overload preprocesses the input with the templated window function.
     *
     * @tparam Projection WindowFunction type.
     * @param samples The transform input.
     * @param projection The instantiated WindowFunction.
     *
     * @return The Result describing the DFT complex coefficients and the accumulated window function.
     *
     * @throws std::invalid_argument The size of the transform input does not match the prepared plan.
     */
    template <detail::WindowFunction Projection>
    [[nodiscard]] Result execute(
            std::span<const float> samples,
            Projection projection
    )
    {
        validate_input_size(samples);

        auto projected = prepare_projection_buffer();
        float scale_divisor = 0.0f;

        for (std::size_t i = 0; i < samples.size(); ++i) {
            const auto window_value = projection(i, samples.size());
            scale_divisor += window_value;
            projected[i] = samples[i] * window_value;
        }

        return {
                .coefficients = execute_from_mutable_input(projected.data(), requested_flags),
                .scale_divisor = scale_divisor,
        };
    }

private:
    /**
     * Verifies that the given sample vector is suitable for transform by the existing metadata.
     *
     * @param samples The transform input candidate.
     *
     * @throws std::invalid_argument The size of the transform input does not match the prepared plan.
     */
    void validate_input_size(std::span<const float> samples) const;

    /**
     * Prepares (or recovers from cache) a buffer suitable to store the projected input.
     *
     * Ensures that @ref projected_input is initialised with a suitably sized real-valued buffer for the transform
     * input.
     *
     * @return A mutable view of @ref projected_input.
     *
     * @throws std::logic_error The output buffer was not prepared, probably due to FFTRealComplex being moved.
     * @throws std::runtime_error The projection buffer could not be allocated.
     */
    [[nodiscard]] std::span<float> prepare_projection_buffer();

    /**
     * Produce the plan and execute the FFTW native functions to compute the DFT.
     *
     * The output is written to @ref output, to which an observing view is returned.
     *
     * @param input Transform input location.
     * @param planner_flags Flags for the FFTW planner.
     *
     * @return An observing view of the DFT coefficients.
     *
     * @throws std::runtime_error The FFTW plan did not already exist and could not be created.
     * @throws std::logic_error The output buffer was not prepared, probably due to FFTRealComplex being moved.
     * @throws std::invalid_argument The given input was null.
     */
    [[nodiscard]] std::span<const fftwf_complex> execute_from_mutable_input(
            float* input,
            unsigned int planner_flags
    );

    /**
     * Ensure that the @ref plan is ready for executing the transform.
     *
     * @param input Transform input location.
     * @param planner_flags Flags for the FFTW planner.
     *
     * @throws std::runtime_error The FFTW plan did not already exist and could not be created.
     */
    void prepare_plan(
            float* input,
            unsigned int planner_flags
    );

    /**
     * Clear any existing FFTW plan and associated metadata.
     */
    void destroy_plan() noexcept;

    /**
     * Destruct internal FFTW state.
     */
    void reset() noexcept;

    std::size_t transform_size = 0;               /**< Number of reals in @ref planned_input. */
    int fftw_transform_size = 0;                  /**< @ref transform_size typed for the FFTW C API. */
    std::size_t output_size = 0;                  /**< Number of complexes in @ref output following DFT. */

    unsigned int requested_flags = FFTW_ESTIMATE; /**< FFTW planner flags requested by the user. */

    float* projected_input = nullptr; /**< Input vector for transform following map under a window function. */
    fftwf_complex* output = nullptr;  /**< Destination for DFT complex coefficients. */

    fftwf_plan plan = nullptr;        /**< Prepared FFTW plan. */
    float* planned_input = nullptr;   /**< Input vector for which the @ref plan was prepared. */
    unsigned int planned_flags = 0;   /**< FFTW planner flags being used by @ref plan. */
};

} // namespace echomap

#endif // ECHOMAP_FFTWREALCOMPLEX_HPP
