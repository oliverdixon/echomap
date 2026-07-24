/**
 * @file
 *
 * EchoMapWeb implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-24
 */

#ifdef __EMSCRIPTEN__

#include "EchoMapWeb.hpp"

#include <emscripten/emscripten.h>

#include "../notifications/AllNotifications.hpp"

#ifndef __EMSCRIPTEN_PTHREADS__
#warning "The Emscripten application will be single-threaded."
#endif // __EMSCRIPTEN_PTHREADS__

namespace echomap
{

void EchoMapWeb::visit_notification(
        Notification& notification
)
{
    std::visit(make_common_notification_visitors(), notification);
}

void EchoMapWeb::render_shim(
        void* const echomap_instance
)
{
    auto* instance = static_cast<EchoMapWeb*>(echomap_instance);
    instance->render();
}

void EchoMapWeb::run_event_loop()
{
    emscripten_set_main_loop_arg(&EchoMapWeb::render_shim, this, 0, true);
}

} // namespace echomap

#endif // __EMSCRIPTEN__
