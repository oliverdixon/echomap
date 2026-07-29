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
            variant_helpers::Overloaded{
                    make_common_notification_visitors(),
            },

            notification
    );
}

} // namespace echomap

#endif // __EMSCRIPTEN__
