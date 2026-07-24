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
    render();
    instance.ProcessEvents();

    while (glfwWindowShouldClose(window) == 0) {
        if (forced_frames > 0) {
            glfwPollEvents();
            --forced_frames;
        } else
            glfwWaitEvents();

        render();
        instance.ProcessEvents();
    }
}

void EchoMapNative::visit_notification(
        Notification& notification
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
    if (active_modal != nullptr) {
        LOG_WARN("Ignoring request to raise file chooser since there is an active modal.");
        return;
    }

    active_modal = std::make_unique<FileChooser>(this, std::move(notification.callback));
}

} // namespace echomap

#endif // __EMSCRIPTEN__
