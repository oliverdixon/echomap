/**
 * @file
 *
 * EchoMapNative specification
 *
 * @author Oliver Dixon
 * @date 2026-07-24
 */

#ifndef ECHOMAP_ECHOMAPNATIVE_HPP
#define ECHOMAP_ECHOMAPNATIVE_HPP

#ifndef __EMSCRIPTEN__

#include "../EchoMap.hpp"

namespace echomap
{

/**
 * EchoMap application implementation for native (non-WebAssembly) platforms.
 */
class EchoMapNative : public EchoMap
{
public:
    void run_event_loop() override;

protected:
    void visit_notification(Notification& notification) override;

private:
    void handle_notification(RaiseFileChooserNotification& task);
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_ECHOMAPNATIVE_HPP
