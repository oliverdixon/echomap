/**
 * @file
 *
 * EchoMapWeb specification
 *
 * @author Oliver Dixon
 * @date 2026-07-24
 */

#ifndef ECHOMAP_ECHOMAPWEB_HPP
#define ECHOMAP_ECHOMAPWEB_HPP

#ifdef __EMSCRIPTEN__

#include "../EchoMap.hpp"

namespace echomap
{

/**
 * EchoMap application implementation for WebAssembly/Emscripten platforms.
 */
class EchoMapWeb : public EchoMap
{
public:
    void run_event_loop() override;

protected:
    void visit_notification(Notification& notification) override;

private:
    /**
     * Invokes the renderer from a static context given an untyped mutable pointer to the EchoMap object instance.
     *
     * This function is intended to be used as a callback from the Emscripten C API.
     *
     * @param echomap_instance The EchoMapWeb application instance on which to invoke the renderer.
     */
    static void render_shim(void* echomap_instance);
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_ECHOMAPWEB_HPP
