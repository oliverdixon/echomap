/**
 * @file
 *
 * FFTWRealToComplex1D implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-26
 */

#include "FFTWRealToComplex1D.hpp"

#include <limits>
#include <stdexcept>
#include <utility>

namespace echomap
{

FFTWRealToComplex1D::FFTWRealToComplex1D(
        const std::size_t requested_transform_size,
        const unsigned int requested_projected_planner_flags
) :
    transform_size(requested_transform_size),
    projected_planner_flags(requested_projected_planner_flags)
{
    if (transform_size == 0)
        throw std::invalid_argument("FFTW transform size must be non-zero.");

    if (transform_size > static_cast<std::size_t>(std::numeric_limits<int>::max()))
        throw std::invalid_argument("FFTW transform size exceeds int range.");

    fftw_transform_size = static_cast<int>(transform_size);
    output_size = transform_size / 2 + 1;

    output = fftwf_alloc_complex(output_size);

    if (output == nullptr)
        throw std::runtime_error("Failed to allocate FFTW output buffer.");
}

FFTWRealToComplex1D::~FFTWRealToComplex1D() noexcept
{
    reset();
}

FFTWRealToComplex1D::FFTWRealToComplex1D(
        FFTWRealToComplex1D&& other
) noexcept :
    transform_size(
            std::exchange(
                    other.transform_size,
                    0
            )
    ),
    fftw_transform_size(
            std::exchange(
                    other.fftw_transform_size,
                    0
            )
    ),
    output_size(
            std::exchange(
                    other.output_size,
                    0
            )
    ),
    projected_planner_flags(
            std::exchange(
                    other.projected_planner_flags,
                    FFTW_ESTIMATE
            )
    ),
    projected_input(
            std::exchange(
                    other.projected_input,
                    nullptr
            )
    ),
    output(std::exchange(
            other.output,
            nullptr
    )),
    plan(std::exchange(
            other.plan,
            nullptr
    )),
    planned_input(
            std::exchange(
                    other.planned_input,
                    nullptr
            )
    ),
    planned_flags(
            std::exchange(
                    other.planned_flags,
                    0
            )
    )
{
}

FFTWRealToComplex1D& FFTWRealToComplex1D::operator=(
        FFTWRealToComplex1D&& other
) noexcept
{
    if (this == &other)
        return *this;

    reset();

    transform_size = std::exchange(other.transform_size, 0);
    fftw_transform_size = std::exchange(other.fftw_transform_size, 0);
    output_size = std::exchange(other.output_size, 0);
    projected_planner_flags = std::exchange(other.projected_planner_flags, FFTW_ESTIMATE);

    projected_input = std::exchange(other.projected_input, nullptr);
    output = std::exchange(other.output, nullptr);

    plan = std::exchange(other.plan, nullptr);
    planned_input = std::exchange(other.planned_input, nullptr);
    planned_flags = std::exchange(other.planned_flags, 0);

    return *this;
}

FFTWRealToComplex1D::Result FFTWRealToComplex1D::execute(
        const std::span<const float> samples
)
{
    validate_input_size(samples);

    /*
     * FFTW's r2c API takes float*, even for an out-of-place transform.
     *
     * The direct no-projection path deliberately creates its plan using
     * FFTW_ESTIMATE so that planning does not modify the caller-provided input.
     */
    auto* const input = const_cast<float*>(samples.data()); // NOLINT(*-pro-type-const-cast)

    return {
            .coefficients = execute_from_mutable_input(input, FFTW_ESTIMATE),
            .scale_divisor = static_cast<float>(transform_size),
    };
}

void FFTWRealToComplex1D::validate_input_size(
        const std::span<const float> samples
) const
{
    if (samples.size() != transform_size)
        throw std::invalid_argument("Input size does not match FFTW transform size.");
}

std::span<float> FFTWRealToComplex1D::projected_input_buffer()
{
    if (output == nullptr)
        throw std::logic_error("Cannot execute a moved-from FFTW transform.");

    if (projected_input == nullptr) {
        projected_input = fftwf_alloc_real(transform_size);

        if (projected_input == nullptr)
            throw std::runtime_error("Failed to allocate FFTW projected input buffer.");
    }

    return {projected_input, transform_size};
}

std::span<const fftwf_complex> FFTWRealToComplex1D::execute_from_mutable_input(
        float* const input,
        const unsigned int planner_flags
)
{
    if (output == nullptr)
        throw std::logic_error("Cannot execute a moved-from FFTW transform.");

    if (input == nullptr)
        throw std::invalid_argument("FFTW input pointer must not be null.");

    ensure_plan(input, planner_flags);

    fftwf_execute_dft_r2c(plan, input, output);

    return {output, output_size};
}

void FFTWRealToComplex1D::ensure_plan(
        float* const input,
        const unsigned int planner_flags
)
{
    if (plan != nullptr && planned_input == input && planned_flags == planner_flags)
        return;

    destroy_plan();

    plan = fftwf_plan_dft_r2c_1d(fftw_transform_size, input, output, planner_flags);

    if (plan == nullptr)
        throw std::runtime_error("Failed to create FFTW real-to-complex plan.");

    planned_input = input;
    planned_flags = planner_flags;
}

void FFTWRealToComplex1D::destroy_plan() noexcept
{
    if (plan != nullptr)
        fftwf_destroy_plan(plan);

    plan = nullptr;
    planned_input = nullptr;
    planned_flags = 0;
}

void FFTWRealToComplex1D::reset() noexcept
{
    destroy_plan();

    if (output != nullptr)
        fftwf_free(output);

    if (projected_input != nullptr)
        fftwf_free(projected_input);

    transform_size = 0;
    fftw_transform_size = 0;
    output_size = 0;
    projected_planner_flags = FFTW_ESTIMATE;

    output = nullptr;
    projected_input = nullptr;
}

} // namespace echomap
