/**
 * @file
 *
 * NativeProjectFilePicker implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-29
 */

#if !defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "NativeProjectFilePicker.hpp"

#include "../../panels/native/FileChooser.hpp"
#include "../../utility/Logger.hpp"
#include "../PanelHost.hpp"
#include "../RenderHost.hpp"

namespace echomap
{

NativeProjectFilePicker::NativeProjectFilePicker(
        PanelHost& panel_host,
        RenderHost& render_host
) :
    panel_host(panel_host),
    render_host(render_host)
{
}

void NativeProjectFilePicker::request_project_file(
        SuccessCallbackT success,
        CancelledCallbackT cancelled
)
{
    if (panel_host.is_modal_shown())
        LOG_WARN("Ignoring request to raise file chooser since there is an active modal.");
    else
        panel_host.change_active_modal(
                std::make_unique<FileChooser>(render_host, std::move(success), std::move(cancelled))
        );
}
} // namespace echomap

#endif // __EMSCRIPTEN__
