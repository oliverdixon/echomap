/**
 * @file
 *
 * WebProjectFilePicker specification
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#ifndef ECHOMAP_WEBPROJECTFILEPICKER_HPP
#define ECHOMAP_WEBPROJECTFILEPICKER_HPP

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "../IProjectFilePicker.hpp"

extern "C" int echomap_on_project_file_picked(const char* path) noexcept;
extern "C" int echomap_on_register_vfs_mapping(
        std::size_t,
        const char*,
        const char*
) noexcept;

namespace echomap
{

/**
 * @todo Document
 */
class WebProjectFilePicker : public IProjectFilePicker
{
public:
    WebProjectFilePicker();
    ~WebProjectFilePicker() noexcept override;

    void request_project_file(
            SuccessCallbackT success,
            CancelledCallbackT cancelled
    ) override;

private:
    friend int ::echomap_on_project_file_picked(const char* path) noexcept;
    friend int ::echomap_on_register_vfs_mapping(
            std::size_t,
            const char*,
            const char*
    ) noexcept;

    static int complete_project_file_pick(const char * path) noexcept;

    SuccessCallbackT success_callback;
    CancelledCallbackT cancelled_callback;

    static WebProjectFilePicker* instance;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_WEBPROJECTFILEPICKER_HPP
