/**
 * @file
 * @brief Wave file specification
 * @author Oliver Dixon
 * @date 2026-06-24
 */

#ifndef ECHOMAP_SIGNALFACTORY_H
#define ECHOMAP_SIGNALFACTORY_H

#include <dr_wav.h>

#include <memory>
#include <span>
#include <vector>

namespace EchoMap
{

class Signal;

/**
 * Provides various convenience functions for constructing Signal objects in exotic ways.
 */
class SignalFactory
{
public:
    /**
     * Loads a WAV file from the file system.
     *
     * The Signal objects are constructed by this function, with default names, in the order of the channels in the wave
     * file. That is, the first Signal will represent the first channel in the file, etc.
     *
     * @param file_path The location of the WAV on the local file system.
     * @throws ConfigurationError The WAV file could not be loaded.
     * @returns Series of owned Signal objects, one for each channel.
     */
    [[nodiscard]] static std::vector<std::unique_ptr<Signal>> load_wave_file(const char* file_path);

    /**
     * Loads a WAV file from the file system.
     *
     * The Signal objects are not constructed by this function. As many constructed Signal objects as there are channels
     * in the wave file must be provided through the mutable Signal range.
     *
     * @param file_path The location of the WAV on the local file system.
     * @param channels Destination of the Signal channels. In particular, a list of pointers to Signal. For each entry
     *  at index <code>idx</code>:
     *  <ul>
     *      <li>If <code>channels[idx] == nullptr</code>, the wave file channel <code>idx</code> is ignored; or</li>
     *      <li>If <code>channels[idx] != nullptr</code>, the wave file channel <code>idx</code> is written to
     *          <code>*channels[idx]</code>.</li>
     *  </ul>
     *
     * @throws ConfigurationError The WAV file could not be loaded.
     * @pre There are sufficient Signal objects in the destination range to store channels in the wave file.
     */
    static void load_wave_file(
            const char* file_path,
            std::span<Signal* const> channels
    );

    /**
     * Downsamples an existing Signal instance across all channels to the given number of samples.
     *
     * @param source The existing Signal to downsample.
     * @param downsample_factor The factor by which the number of samples should be reduced during downsampling.
     * @param name Optional display name. If omitted, a sensible default based on the source signal and downsampling
     *  factor will be used.
     */
    [[nodiscard]] static std::unique_ptr<Signal> downsample(
            const Signal& source,
            float downsample_factor,
            std::string_view name = {}
    );

private:
    /**
     * Loads the time-series sampled data into the given Signal objects.
     *
     * @param drwav_info The initialised and loaded dr_wav control structure.
     * @param file_path The location of the WAV on the local file system.
     * @param channels Destination of the Signal channels.
     *
     * @pre There are sufficient Signal objects in the destination range to store channels in the wave file.
     * @see load_wave_file(const char*, mutable_signal_range auto&&) for semantics of the destination channels
     *  parameter.
     */
    static void load_wave_file_into_channels(
            drwav& drwav_info,
            std::string_view file_path,
            std::span<Signal* const> channels
    );

    /**
     * Create a new Signal by downsampling the data points of an existing Signal to the given threshold.
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
     * @param source The original Signal to be downsampled.
     * @param threshold The number of samples in the downsampled data.
     * @param name Name of the downsampled Signal.
     * @return The downsampled Signal, detained in an owning container.
     * @post The number of samples in the returned signal matches the threshold parameter.
     */
    [[nodiscard]] static std::unique_ptr<Signal> lttb_downsample(
            const Signal& source,
            size_t threshold,
            std::string_view name
    );
};

} // namespace EchoMap

#endif // ECHOMAP_SIGNALFACTORY_H
