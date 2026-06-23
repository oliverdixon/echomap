//
// Created by owd on 22/06/2026.
//

#ifndef WEBCFD_WAVDATA_H
#define WEBCFD_WAVDATA_H

#include <cstdint>
#include <span>

namespace WebCFD
{

class WAVData
{
public:
    explicit WAVData(const char * file_path);

    ~WAVData() noexcept;

    [[nodiscard]] const float * get_data() const noexcept;

    [[nodiscard]] std::span<const float> observe_data() const noexcept;

    [[nodiscard]] std::uint64_t get_frame_count() const noexcept;

private:
    float * sample_data = nullptr;
    std::uint64_t pcm_frame_count = 0;
};

} // namespace WebCFD

#endif // WEBCFD_WAVDATA_H
