/**
 * @file
 * @brief AudioChannel class implementation
 * @author Oliver Dixon
 * @date 2026-06-24
 */

#include "AudioChannel.hpp"

#include <cassert>
#include <cmath>
#include <stdexcept>

namespace WebCFD
{

AudioChannel::AudioChannel(
        const AudioChannel& source,
        const std::uint64_t sample_count
)
{
    downsample_and_copy(source, sample_count);
}

AudioChannel::AudioChannel(
        const AudioChannel& source,
        const float downsample_factor
) :
    AudioChannel(source, static_cast<std::uint64_t>(static_cast<float>(source.get_sample_count()) / downsample_factor))
{
}

void AudioChannel::add_sample(
        const Sample& sample
)
{
    if (!samples.empty() && sample.time <= samples.back().time)
        throw std::runtime_error("Inserted sample violates monotonically increasing invariant of channel.");

    samples.push_back(sample);
}

void AudioChannel::emplace_sample(
        const float time,
        const float amplitude
)
{
    if (!samples.empty() && time <= samples.back().time)
        throw std::runtime_error("Inserted sample violates monotonically increasing invariant of channel.");

    samples.emplace_back(time, amplitude);
}

void AudioChannel::reserve_samples(
        const std::size_t count
)
{
    samples.reserve(count);
}

std::uint64_t AudioChannel::get_sample_count() const
{
    return samples.size();
}

std::vector<AudioChannel::Sample>::iterator AudioChannel::begin()
{
    return samples.begin();
}

std::vector<AudioChannel::Sample>::iterator AudioChannel::end()
{
    return samples.end();
}

std::vector<AudioChannel::Sample>::const_iterator AudioChannel::begin() const
{
    return samples.begin();
}

std::vector<AudioChannel::Sample>::const_iterator AudioChannel::end() const
{
    return samples.end();
}

std::vector<AudioChannel::Sample>::const_iterator AudioChannel::cbegin() const noexcept
{
    return samples.cbegin();
}

std::vector<AudioChannel::Sample>::const_iterator AudioChannel::cend() const noexcept
{
    return samples.cend();
}

void AudioChannel::downsample_and_copy(
        const AudioChannel& source_channel,
        const size_t threshold
)
{
    auto& dest = samples;
    const auto& source = source_channel.samples;

    // We don't need to assert for the post-condition on these first two trivial cases.

    if (threshold == 0 || source.size() == 0)
        return;

    if (threshold >= source.size()) {
        dest.reserve(source.size());
        std::ranges::copy_n(source.begin(), source.size(), std::back_inserter(dest));
        return;
    }

    dest.reserve(threshold);

    if (threshold == 1 || threshold == 2) {
        assert(dest.size() == threshold);
        dest = source;
        return;
    }

    const auto bucket_size = static_cast<double>(source.size() - 2) / static_cast<double>(threshold - 2);
    std::size_t fixed_point_idx = 0;

    dest.push_back(source.front()); // // Always add the first point.

    for (std::size_t i = 0; i < threshold - 2; ++i) {
        float average_time = 0.0f;
        float average_amplitude = 0.0f;

        // Calculate the point-average for the next bucket, containing our fixed point.

        const std::size_t average_range_start = (i + 1) * bucket_size + 1;
        const auto average_range_end = std::min(static_cast<std::size_t>((i + 2) * bucket_size + 1), source.size());
        const auto average_range_length = average_range_end - average_range_start;

        for (std::size_t range_idx = average_range_start; range_idx < average_range_end; ++range_idx) {
            average_time += source[average_range_start].time;
            average_amplitude += source[average_range_start].amplitude;
        }

        average_time /= average_range_length;
        average_amplitude /= average_range_length;

        // Get the range for the current bucket and compute triangle areas over the three buckets.

        const std::size_t range_lower = i * bucket_size + 1;
        const std::size_t range_upper = (i + 1) * bucket_size + 1;

        const float fixed_point_time = source[fixed_point_idx].time;
        const float fixed_point_amplitude = source[fixed_point_idx].time;
        auto max_area = std::numeric_limits<float>::lowest();
        std::size_t next_fixed_point_idx = 0;

        for (std::size_t range_idx = range_lower; range_idx < range_upper; ++range_idx) {
            // Calculate triangle area formed by the vertices in the adjacent buckets, tracking the maximum.
            const float area = std::abs(
                (fixed_point_time - average_time) * (source[range_idx].amplitude - fixed_point_amplitude) -
                (fixed_point_time - source[range_idx].time) * (average_amplitude - fixed_point_amplitude)
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
        dest.push_back(source[next_fixed_point_idx]);
        fixed_point_idx = next_fixed_point_idx;
    }

    dest.push_back(source.back()); // Always add the last point.
    assert(dest.size() == threshold);
}

} // namespace WebCFD
