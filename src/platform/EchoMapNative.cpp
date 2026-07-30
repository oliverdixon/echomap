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

#include "../services/native/FullProjectController.hpp"
#include "../services/native/NativeProjectFilePicker.hpp"

namespace echomap
{

EchoMapNative::EchoMapNative() :
    EchoMap([] {
        glfwPostEmptyEvent();
    })
{
    setup_controller(
            std::make_unique<FullProjectController>(
                    std::make_unique<NativeProjectFilePicker>(panel_host, render_host),
                    panel_host,
                    worker
            )
    );
}

void EchoMapNative::run_event_loop()
{
    process_worker_results();
    render_host.render(panel_host);
    render_host.process_instance_events();

    while (render_host.wait_for_frame_trigger()) {
        process_worker_results();
        render_host.render(panel_host);
        render_host.process_instance_events();
    }
}

} // namespace echomap

#endif // __EMSCRIPTEN__
