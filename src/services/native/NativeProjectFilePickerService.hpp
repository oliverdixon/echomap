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
 * @todo Document
 */
class NativeProjectFilePickerService : public IFilePickerService
{
public:
    explicit NativeProjectFilePickerService(
            PanelHost& panel_host,
            RenderHost& render_host
    );

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
