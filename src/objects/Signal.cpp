/**
 * @file
 * @brief Audio signal class implementation
 * @author Oliver Dixon
 * @date 2026-06-24
 */

#include "Signal.hpp"

#include <cassert>
#include <cmath>

#include "../Logger.hpp"

namespace EchoMap
{

template <> constexpr std::string Object<Signal>::class_name = "Signal";

Signal::Signal(
        const std::string_view name,
        const std::optional<Source>& source
) :
    Object(name),
    fs_source(source)
{
}

Signal::Signal(
        const Signal& source,
        const std::uint64_t sample_count,
        const std::string_view name
) :
    Object(name),
    timing_baseline(source.timing_baseline),
    fs_source(source.fs_source)
{
    downsample_and_copy(source, sample_count);

    if (fs_source.has_value())
        fs_source->dirty = true;
}

Signal::Signal(
        const Signal& source,
        const float downsample_factor,
        const std::string_view name
) :
    Object(name),
    timing_baseline(source.timing_baseline),
    fs_source(source.fs_source)
{
    auto sample_count = static_cast<std::uint64_t>(static_cast<float>(source.get_sample_count()) / downsample_factor);

    if (sample_count < downsample_factor)
        sample_count = static_cast<std::uint64_t>(downsample_factor);

    downsample_and_copy(source, sample_count);

    if (fs_source.has_value())
        fs_source->dirty = true;
}

void Signal::emplace_sample(
        const float amplitude
)
{
    samples.emplace_back(amplitude);
    if (time_offsets.has_value())
        time_offsets->emplace_back(0);

    if (fs_source.has_value())
        fs_source->dirty = true;
}

void Signal::emplace_sample(
        const float time,
        const float amplitude
)
{
    samples.emplace_back(amplitude);
    emplace_time(time);

    if (fs_source.has_value())
        fs_source->dirty = true;
}

void Signal::emplace_sample_from_source(
        const float amplitude
)
{
    samples.emplace_back(amplitude);
    if (time_offsets.has_value())
        time_offsets->emplace_back(0);
}

void Signal::emplace_sample_from_source(
        const float time,
        const float amplitude
)
{
    samples.emplace_back(amplitude);
    emplace_time(time);
}

void Signal::reserve_samples(
        const std::size_t count
)
{
    samples.reserve(count);

    if (time_offsets.has_value())
        time_offsets->reserve(count);
}

std::uint64_t Signal::get_sample_count() const noexcept
{
    return samples.size();
}

const std::optional<Signal::Source>& Signal::observe_source() const noexcept
{
    return fs_source;
}

void Signal::set_source(
        const std::filesystem::path& path,
        const std::size_t channel
)
{
    fs_source = Source(path, channel);
}

float Signal::get_time_offset() const noexcept
{
    return timing_baseline.time_offset;
}

void Signal::set_time_offset(
        const float new_time_offset
) noexcept
{
    timing_baseline.time_offset = new_time_offset;
}

std::size_t Signal::get_sample_rate() const noexcept
{
    return timing_baseline.sample_rate;
}

void Signal::set_sample_rate(
        const std::size_t new_sample_rate
) noexcept
{
    timing_baseline.sample_rate = new_sample_rate;
    timing_baseline.sample_rate_r = new_sample_rate == 0 ? 0 : 1.0f / new_sample_rate;
}

decltype(Signal::samples)::const_iterator Signal::begin() const
{
    return samples.begin();
}

decltype(Signal::samples)::const_iterator Signal::end() const
{
    return samples.end();
}

decltype(Signal::samples)::const_iterator Signal::cbegin() const noexcept
{
    return samples.cbegin();
}

decltype(Signal::samples)::const_iterator Signal::cend() const noexcept
{
    return samples.cend();
}

bool Signal::is_uniformly_sampled() const noexcept
{
    return !time_offsets.has_value();
}

Signal::Signal(
        const Signal& old_signal
) :
    Object(CopyTag{},
           old_signal),
    samples(old_signal.samples),
    timing_baseline(old_signal.timing_baseline),
    fs_source(old_signal.fs_source),
    time_offsets(old_signal.time_offsets)
{
}

Signal::Signal(
        const Signal& old_signal,
        const std::string_view new_name
) :
    Object(CopyTag{},
           old_signal,
           new_name),
    samples(old_signal.samples),
    timing_baseline(old_signal.timing_baseline),
    fs_source(old_signal.fs_source),
    time_offsets(old_signal.time_offsets)
{
}

float Signal::get_time_at_index(
        const std::size_t index
) const noexcept
{
    assert(index < samples.size());
    const float baseline_time = timing_baseline.time_offset + static_cast<float>(index) * timing_baseline.sample_rate_r;
    return time_offsets.has_value() ? baseline_time + (*time_offsets)[index] : baseline_time;
}

void Signal::emplace_time(
        const float given_time
)
{
    assert(!samples.empty());

    constexpr float epsilon = 1.0e-6f;
    const auto expected = timing_baseline.time_offset + (samples.size() - 1) * timing_baseline.sample_rate_r;

    // Check the time derived from the baseline, as if we're continuing with a uniformly sampled signal.
    if (const auto offset = given_time - expected; std::abs(offset) <= epsilon) {
        /*
         * If the given time matches what we expect, we don't need to do anything. Only update the explicit offsets
         * (with an offset of zero) if they're already there.
         */
        if (time_offsets.has_value())
            time_offsets->emplace_back(0.0f);
    } else {
        /*
         * If the given time doesn't match what we expect, then a variably sampled entry has been introduced. If the
         * signal is newly variable, bad news!
         */

        if (time_offsets.has_value())
            time_offsets->emplace_back(offset);
        else {
            LOG_F_DEBUG("Signal \"{}\" is no longer uniformly sampled.", get_name());
            time_offsets.emplace(samples.size(), 0.0f);
            time_offsets->back() = offset;
        }
    }
}

void Signal::downsample_and_copy(
        const Signal& source_channel,
        const size_t threshold
)
{
    auto& dest = samples;
    const auto& source = source_channel.samples;

    samples.clear();
    time_offsets.reset();

    // We don't need to assert for the post-condition on these trivial cases.

    if (threshold == 0 || source.empty())
        return;

    if (threshold >= source.size()) {
        samples = source_channel.samples;
        time_offsets = source_channel.time_offsets;
        return;
    }

    dest.reserve(threshold);

    if (threshold == 1) {
        emplace_sample(source_channel.get_time_at_index(0), source.front());
        return;
    }

    if (threshold == 2) {
        emplace_sample(source_channel.get_time_at_index(0), source.front());
        emplace_sample(source_channel.get_time_at_index(source.size() - 1), source.back());
        return;
    }

    const double bucket_size = static_cast<double>(source.size() - 2) / static_cast<double>(threshold - 2);
    std::size_t fixed_point_idx = 0;

    // Always add the first point.
    emplace_sample(source_channel.get_time_at_index(0), source.front());

    for (std::size_t i = 0; i < threshold - 2; ++i) {
        float average_time = 0.0f;
        float average_amplitude = 0.0f;

        // Calculate the point-average for the next bucket, containing our fixed point.

        const auto average_range_start = static_cast<std::size_t>(std::floor((i + 1) * bucket_size)) + 1;
        const auto average_range_end =
                std::min(static_cast<std::size_t>(std::floor((i + 2) * bucket_size)) + 1, source.size());
        const auto average_range_length = average_range_end - average_range_start;

        for (auto range_idx = average_range_start; range_idx < average_range_end; ++range_idx) {
            average_time += source_channel.get_time_at_index(range_idx);
            average_amplitude += source[range_idx];
        }

        average_time /= static_cast<float>(average_range_length);
        average_amplitude /= static_cast<float>(average_range_length);

        // Store the sample data at the fixed point.
        const auto fp_time = source_channel.get_time_at_index(fixed_point_idx);
        const auto fp_amplitude = source[fixed_point_idx];

        // Get the range for the current bucket and compute triangle areas over the three buckets.
        const auto range_lower = static_cast<std::size_t>(std::floor(i * bucket_size)) + 1;
        const auto range_upper =
                std::min(static_cast<std::size_t>(std::floor((i + 1) * bucket_size)) + 1, source.size() - 1);

        auto max_area = std::numeric_limits<float>::lowest();
        auto next_fixed_point_idx = range_lower;

        // Calculate triangle area formed by the vertices in the adjacent buckets, tracking the maximum.
        for (auto range_idx = range_lower; range_idx < range_upper; ++range_idx) {
            const float area = std::abs(
                    (fp_time - average_time) * (source[range_idx] - fp_amplitude) -
                    (fp_time - source_channel.get_time_at_index(range_idx)) * (average_amplitude - fp_amplitude)
            );

            if (area > max_area) {
                max_area = area;
                next_fixed_point_idx = range_idx;
            }
        }

        /*
         * Pick the point from the bucket to include in the downsampled data, and set the index as our next starting
         * point.
         */
        emplace_sample(source_channel.get_time_at_index(next_fixed_point_idx), source[next_fixed_point_idx]);
        fixed_point_idx = next_fixed_point_idx;
    }

    // Always add the last point.
    emplace_sample(source_channel.get_time_at_index(source.size() - 1), source.back());

    assert(dest.size() == threshold);
}

} // namespace EchoMap
