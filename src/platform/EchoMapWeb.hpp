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

#if defined(__EMSCRIPTEN__) || defined(__DOXYGEN__)

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

    EchoMapWeb();
    ~EchoMapWeb() override;

protected:
    void visit_notification(Notification& notification) override;

    void handle_result(LoadProjectResult&& result) override;
    void handle_result(LoadSignalFileResult&& result) override;

private:
    void handle_notification(const CancelProjectLoadNotification& notification);
    void handle_notification(const CompleteProjectLoadNotification& notification);
    void handle_notification(RegisterVFSMappingNotification& notification) const;

    /**
     * Invokes the renderer from a static context given an untyped mutable pointer to the EchoMap object instance.
     *
     * This function is intended to be used as a callback from the Emscripten C API.
     *
     * @param echomap_instance The EchoMapWeb application instance on which to invoke the renderer.
     */
    static void render_shim(void* echomap_instance);

    std::unique_ptr<PartialProject> unloaded_project; /**< Owning container for the unloaded Project. */
};

} // namespace echomap

#endif // __EMSCRIPTEN__

#endif // ECHOMAP_ECHOMAPWEB_HPP
