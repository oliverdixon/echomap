//
// Created by owd on 22/06/2026.
//

#include "WAVData.hpp"

#define DR_WAV_IMPLEMENTATION
#include <dr_wav.h>

#include <cstring>
#include <limits>

#include "ConfigurationError.hpp"

namespace WebCFD {

WAVData::WAVData(
        const char* const file_path
)
{
    float * mutable_sample_data = nullptr;
    unsigned int channels;
    unsigned int sample_rate;
    drwav_uint64 unbounded_frame_count;

    mutable_sample_data = drwav_open_file_and_read_pcm_frames_f32(file_path, &channels, &sample_rate,
        &unbounded_frame_count, nullptr);

    if (mutable_sample_data == nullptr)
        throw ConfigurationError("Cannot open and read WAV file: " + std::string(strerror(errno)));

    using FrameCountT = decltype(pcm_frame_count);

    if (unbounded_frame_count > std::numeric_limits<FrameCountT>::max()) {
        drwav_free(sample_data, nullptr);
        throw ConfigurationError("Given WAV file contained too many PCM frames.");
    }

    pcm_frame_count = static_cast<FrameCountT>(unbounded_frame_count);
    sample_data = mutable_sample_data;
}

WAVData::~WAVData() noexcept
{
    drwav_free(sample_data, nullptr);
}

const float* WAVData::get_data() const noexcept
{
    return sample_data;
}

std::span<const float> WAVData::observe_data() const noexcept
{
    return { sample_data, pcm_frame_count };
}

std::uint64_t WAVData::get_frame_count() const noexcept
{
    return pcm_frame_count;
}

} // WebCFD
