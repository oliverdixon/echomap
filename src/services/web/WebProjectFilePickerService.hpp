/**
 * @file
 *
 * WebProjectFilePickerService specification
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#ifndef ECHOMAP_WEBPROJECTFILEPICKERSERVICE_HPP
#define ECHOMAP_WEBPROJECTFILEPICKERSERVICE_HPP

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "../IFilePickerService.hpp"

extern "C" int echomap_on_project_file_picked(const char* path) noexcept;

namespace echomap
{

/**
 * @todo Document
 */
class WebProjectFilePickerService : public IFilePickerService
{
public:
    WebProjectFilePickerService();
    ~WebProjectFilePickerService() noexcept override;

    void request_project_file(
            SuccessCallbackT success,
            CancelledCallbackT cancelled
    ) override;

private:
    friend int ::echomap_on_project_file_picked(const char* path) noexcept;

    static int complete_project_file_pick(const char * path) noexcept;

    SuccessCallbackT success_callback;
    CancelledCallbackT cancelled_callback;

    static WebProjectFilePickerService* instance;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_WEBPROJECTFILEPICKERSERVICE_HPP
