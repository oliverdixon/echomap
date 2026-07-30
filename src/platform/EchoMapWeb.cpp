/**
 * @file
 *
 * EchoMapWeb implementation
 *
 * @author Oliver Dixon
 * @date 2026-07-24
 */

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

#include "EchoMapWeb.hpp"

#include <emscripten/emscripten.h>

#include "../services/web/PartialProjectController.hpp"

#ifndef __EMSCRIPTEN_PTHREADS__
#warning "The Emscripten application will be single-threaded."
#endif // __EMSCRIPTEN_PTHREADS__

namespace echomap
{

EchoMapWeb::EchoMapWeb()
{
    setup_controller(std::make_unique<PartialProjectController>(panel_host, worker));
}

void EchoMapWeb::render_shim(
        void* const echomap_instance
)
{
    auto* const instance = static_cast<EchoMapWeb*>(echomap_instance);

    instance->process_worker_results();
    instance->render_host.render(instance->panel_host);
}

void EchoMapWeb::run_event_loop()
{
    emscripten_set_main_loop_arg(&EchoMapWeb::render_shim, this, 0, true);
}

} // namespace echomap

#endif // __EMSCRIPTEN__
