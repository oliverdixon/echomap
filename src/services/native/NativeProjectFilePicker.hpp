/**
 * @file
 *
 * NativeProjectFilePicker specification
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#ifndef ECHOMAP_NATIVEPROJECTFILEPICKER_HPP
#define ECHOMAP_NATIVEPROJECTFILEPICKER_HPP

#if !defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "../IProjectFilePicker.hpp"

namespace echomap
{

class PanelHost;
class RenderHost;

/**
 * @todo Document
 */
class NativeProjectFilePicker : public IProjectFilePicker
{
public:
    explicit NativeProjectFilePicker(
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

#endif // ECHOMAP_NATIVEPROJECTFILEPICKER_HPP
