/**
 * @file
 *
 * NativeProjectFilePickerService specification
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#ifndef ECHOMAP_NATIVEPROJECTFILEPICKERSERVICE_HPP
#define ECHOMAP_NATIVEPROJECTFILEPICKERSERVICE_HPP

#if !defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "../IFilePickerService.hpp"

namespace echomap
{

class PanelHost;
class RenderHost;

/**
 * Provides a native-platform implementation for opening a file selection dialog.
 */
class NativeProjectFilePickerService : public IFilePickerService
{
public:
    explicit NativeProjectFilePickerService(
            PanelHost& panel_host,
            RenderHost& render_host
    );

    ~NativeProjectFilePickerService() override = default;

    NativeProjectFilePickerService(const NativeProjectFilePickerService&) = delete;
    NativeProjectFilePickerService& operator=(const NativeProjectFilePickerService&) = delete;
    NativeProjectFilePickerService(NativeProjectFilePickerService&&) = delete;
    NativeProjectFilePickerService& operator=(NativeProjectFilePickerService&&) = delete;

    void request_project_file(
            SuccessCallbackT success,
            CancelledCallbackT cancelled
    ) override;

private:
    PanelHost& panel_host;
    RenderHost& render_host;
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_NATIVEPROJECTFILEPICKERSERVICE_HPP
