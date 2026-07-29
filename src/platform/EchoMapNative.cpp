/**
 * @file
 *
 * EchoMapNative implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-24
 */

#if !defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "EchoMapNative.hpp"

#include "../notifications/AllNotifications.hpp"
#include "../panels/native/FileChooser.hpp"
#include "../utility/Logger.hpp"

namespace echomap
{

void EchoMapNative::run_event_loop()
{
    tick();
    render_host.render(panel_host);
    render_host.process_instance_events();

    while (render_host.wait_for_frame_trigger()) {
        tick();
        render_host.render(panel_host);
        render_host.process_instance_events();
    }
}

void EchoMapNative::visit_notification(
        Notification notification
)
{
    std::visit(
            // clang-format off

            variant_helpers::Overloaded{
                make_common_notification_visitors(),
                [this](RaiseFileChooserNotification& n) { handle_notification(n); },
            },

            // clang-format on
            notification
    );
}

void EchoMapNative::handle_notification(
        RaiseFileChooserNotification& notification
)
{
    if (panel_host.is_modal_shown())
        LOG_WARN("Ignoring request to raise file chooser since there is an active modal.");
    else
        panel_host.change_active_modal(
                std::make_unique<FileChooser>(
                        render_host,
                        std::move(notification.success_callback),
                        std::move(notification.cancelled_callback)
                )
        );
}

} // namespace echomap

#endif // __EMSCRIPTEN__
