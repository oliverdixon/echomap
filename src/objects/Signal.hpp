/**
 * @file
 * @brief Audio signal class specification
 * @author Oliver Dixon
 * @date 2026-06-24
 */

#ifndef ECHOMAP_SIGNAL_HPP
#define ECHOMAP_SIGNAL_HPP

#include <cstdint>
#include <filesystem>
#include <ranges>
#include <vector>

#include "Object.hpp"

namespace EchoMap
{

/**
 * A single channel of discretely sampled audio data.
 *
 * Signals always store their sampled time series in memory, but they optionally express an external source. This is
 * currently a file specification on the local file system, and an internal channel number, indicating the origin of the
 * sample data.
 *
 * @invariant Amplitude samples are stored contiguously in memory. Timing information may either be inferred from the
 *  baseline sampling model or adjusted by an optional per-sample offset array. The timed samples range is a lazy
 *  logical view and is not contiguous.
 */
class Signal : public Object<Signal>
{
    std::vector<float> samples; /**< Amplitude sample stream. */

public:
    /**
     * Indicates an external source of a Signal on the filesystem.
     */
    struct Source
    {
        std::filesystem::path path; /**< The path (absolute or relative to CWD) of the source wave file. */
        std::size_t channel;        /**< The channel number of the Signal within the given file. */
        bool dirty = false;         /**< Does the Signal contain additional samples? */
    };

    /**
     * A PCM float-32 sampled audio point at an explicit time offset.
     */
    struct Sample
    {
        float time;      /**< Time, in ms. */
        float amplitude; /**< Amplitude at the time, normalised in the range [-1, 1]. */
    };

    /**
     * Creates an empty optionally named Signal.
     *
     * @param name Optional display name.
     * @param source Optional file system source path indicating the origin of the file.
     */
    explicit Signal(
            std::string_view name = {},
            const std::optional<Source>& source = {}
    );

    /**
     * Downsamples an existing Signal instance across all channels to the given number of samples.
     *
     * @param source The existing Signal to downsample.
     * @param sample_count The desired number of samples in the downsampled data.
     * @param name Optional display name.
     */
    Signal(const Signal& source,
           std::uint64_t sample_count,
           std::string_view name = {});

    /**
     * Downsamples an existing Signal instance across all channels by the given factor.
     *
     * If the downsampling factor would cause the downsampled series to consist of fewer samples than the factor, the
     * source is copied and no downsampled is performed.
     *
     * @param source The existing Signal to downsample.
     * @param downsample_factor The factor by which the number of samples should be reduced during downsampling.
     * @param name Optional display name.
     */
    Signal(const Signal& source,
           float downsample_factor,
           std::string_view name = {});

    /**
     * Emplace a sample to the back of the channel sample data.
     *
     * @param amplitude Amplitude of the sample to insert
     */
    void emplace_sample(float amplitude);
    void emplace_sample(
            float time,
            float amplitude
    );

    /**
     * Emplace an externally sourced sample to the back of the channel sample data.
     *
     * @param amplitude Amplitude of the sample to insert
     */
    void emplace_sample_from_source(float amplitude);
    void emplace_sample_from_source(
            float time,
            float amplitude
    );

    /**
     * Reserves memory to store the given number of total samples in the channel.
     *
     * @param count The number of Sample objects to pre-allocate.
     */
    void reserve_samples(std::size_t count);

    /**
     * Retrieves the total number of samples in the Signal stream.
     *
     * @return The number of samples detained by the Signal.
     */
    [[nodiscard]] std::uint64_t get_sample_count() const noexcept;

    /**
     * Retrieves the optional Source of the Signal.
     *
     * <p>
     *  The return value of this operation can be used to determine the origin of the Signal: if the optional is empty,
     *  the signal should be considered "embedded". Otherwise, it has an origin on the filesystem at the given
     *  SignalSource.
     * </p>
     * <p>
     *  If the Signal has an external source, the stored samples in the Signal object do not necessarily match the
     *  external file, as callers may have invoked @ref add_sample with arbitrary samples. The Source should only be
     *  considered a hint. This can be checked by inspecting the Source::dirty flag.
     * </p>
     *
     * @return The Source of the Signal, or an empty optional if the Signal is not externally sourced.
     */
    [[nodiscard]] const std::optional<Source>& observe_source() const noexcept;

    void set_source(
            const std::filesystem::path& path,
            std::size_t channel
    );

    [[nodiscard]] float get_time_offset() const noexcept;
    void set_time_offset(float new_time_offset) noexcept;

    [[nodiscard]] std::size_t get_sample_rate() const noexcept;
    void set_sample_rate(std::size_t new_sample_rate) noexcept;

    [[nodiscard]] decltype(samples)::const_iterator begin() const;
    [[nodiscard]] decltype(samples)::const_iterator end() const;
    [[nodiscard]] decltype(samples)::const_iterator cbegin() const noexcept;
    [[nodiscard]] decltype(samples)::const_iterator cend() const noexcept;

    [[nodiscard]] auto timed_samples() const
    {
        return std::views::iota(std::size_t{0}, samples.size()) |
               std::views::transform([this](const std::size_t index) -> Sample {
                   return {.time = get_time_at_index(index), .amplitude = samples[index]};
               });
    }

    [[nodiscard]] bool is_uniformly_sampled() const noexcept;

    /**
     * Determines the corresponding time of the amplitude appearing at the given index in the sample array.
     *
     * @param index The index of the amplitude in the sample array.
     * @return The timestamp corresponding to the referenced sample.
     *
     * @pre The index is within the bounds of the sample array.
     */
    [[nodiscard]] float get_time_at_index(std::size_t index) const noexcept;

    Signal(const Signal& old_signal);
    Signal(const Signal& old_signal,
           std::string_view new_name);

private:
    /**
     * Provides basic timing information relating to the Signal samples.
     */
    struct Baseline
    {
        float time_offset = 0.0f;    /**< Timestamp, in seconds, of the first sample. */
        std::size_t sample_rate = 0; /**< Constant sample rate, in Hz, of the signal. */
        float sample_rate_r = 0.0f;  /**< Reciprocal of the sample rate; zero if sample rate is zero. */
    };

    void emplace_time(float given_time);

    /**
     * Downsample the data points of a Signal to the given threshold.
     *
     * <p>
     *  This helper uses the well-known Largest-Triangle Three-Buckets (LTTB) downsampling algorithm, described in
     *  detail by the Master's Thesis <a href="https://skemman.is/handle/1946/15343"><i>Downsampling Time Series for
     *  Visual Representation</i>, Sveinn Steinarsson (2013)</a>.
     * </p>
     * <p>
     *  In addition to the thesis, reference implementations in all major languages are available online. This function
     *  uses a specialised implementation for the AudioPoint structure:
     *  https://github.com/sveinn-steinarsson/flot-downsample.
     * </p>
     *
     * @param source_channel The original Signal to be downsampled.
     * @param threshold The number of samples in the downsampled data.
     * @return The downsampled Signal.
     * @post The number of samples in the returned signal matches the threshold parameter.
     *
     * @todo Move to the SignalFactory with FFT.
     */
    void downsample_and_copy(
            const Signal& source_channel,
            size_t threshold
    );

    Baseline timing_baseline;        /**< A baseline of timing parameters. */
    std::optional<Source> fs_source; /**< External source, if any, of the Signal Sample stream. */

    /**
     * An optional vector of explicit timestamps for each sample.
     *
     * <p>
     *  If provided, the timestamps provide a sample-by-sample indication of the time corresponding with amplitudes in
     *  the sample time series data. The scalars are specified as offsets relative to the baseline time, which is
     *  computed as @f$ b_i = t_0 + i \delta t @f$, where @f$ t_0 @f$ is the global time offset, @f$ \delta t @f$ is the
     *  reciprocal of the sample rate, and @f$ i @f$ is the integer zero-based index such that
     *  @f$ 0 < i \leq \vert S \vert @f$, where @f$ \vert S \vert @f$ denotes the size of the sample set @f$ S @f$.
     * </p>
     * <p>
     *  This is useful to support variably sampled time series, such as in the result of LTTB downsampling. Whether
     *  explicit timestamps are provided is rather opaque to the user: they can request timing information, which may be
     *  inferred from the baseline or indexed from this array as appropriate.
     * </p>
     * <p>
     *  When a new sample is emplaced, a time can be provided. If the given time is different than what would be
     *  expected from the baseline, this vector is updated with the suitable offset. If it doesn't exist, it is created.
     * </p>
     *
     * @invariant Timestamp elements are in bijective correspondence with the amplitude samples.
     */
    std::optional<std::vector<float>> time_offsets;
};

} // namespace EchoMap

#endif // ECHOMAP_SIGNAL_HPP
