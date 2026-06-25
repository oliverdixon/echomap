/**
 * @file
 * @brief Wave file specification
 * @author Oliver Dixon
 * @date 2026-06-24
 */

#ifndef WEBCFD_WAVDATA_H
#define WEBCFD_WAVDATA_H

#include <vector>

#include "../Object.hpp"
#include "Signal.hpp"

namespace WebCFD
{

/**
 * A container for wave audio file data composed of one or many channels at a common sample rate.
 */
class WAVData : public Object<WAVData>
{
public:
    /**
     * Loads a WAV file from the file system.
     *
     * @param file_path The location of the WAV on the local file system.
     * @throws ConfigurationError The WAV file could not be loaded.
     */
    explicit WAVData(const char * file_path);

    /**
     * Retrieves the common sample rate of the WAV file.
     *
     * @return The sample rate, in Hz, of the wave data.
     */
    [[nodiscard]] std::uint64_t get_sample_count() const;

    [[nodiscard]] std::vector<Signal>::iterator begin();

    [[nodiscard]] std::vector<Signal>::iterator end();

    [[nodiscard]] std::vector<Signal>::const_iterator begin() const;

    [[nodiscard]] std::vector<Signal>::const_iterator end() const;

    [[nodiscard]] std::vector<Signal>::const_iterator cbegin() const noexcept;

    [[nodiscard]] std::vector<Signal>::const_iterator cend() const noexcept;

private:
    std::vector<Signal> channels;
};

} // namespace WebCFD

#endif // WEBCFD_WAVDATA_H
