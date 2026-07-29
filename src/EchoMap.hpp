/**
 * @file
 * @brief EchoMap class specification
 * @author Oliver Dixon
 * @date 2026-05-05
 */

#ifndef ECHOMAP_ECHOMAP_HPP
#define ECHOMAP_ECHOMAP_HPP

#include <sigc++/scoped_connection.h>

#include "async/Worker.hpp"
#include "async/WorkerResultDespatcher.hpp"
#include "notifications/AllNotificationsFwd.hpp"
#include "services/INotificationSink.hpp"
#include "services/PanelHost.hpp"
#include "services/RenderHost.hpp"

#ifdef __EMSCRIPTEN__
#include "services/web/PartialProjectController.hpp"
#else
#include "services/native/FullProjectController.hpp"
#endif

/**
 * The main EchoMap outermost namespace for all non-exported symbols.
 */
namespace echomap
{

class Project;

/**
 * The EchoMap maintains state for the application including WebGPU and Dear ImGui context, encapsulating
 * initialisation, game loop, interaction, and clean-up.
 */
class EchoMap : public INotificationSink
{
public:
    /**
     * Initialise a EchoMap application instance.
     *
     * Initialisation is a computationally substantial task. Context from all managed frameworks must be initialised
     * (GLFW, WebGPU/Dawn, and Dear ImGui) and their components registered. Once the constructor has completed, the game
     * loop can begin with @ref run_event_loop.
     *
     * @throws ConfigurationError Some part of initialisation, described in the exception message, did not succeed.
     */
    EchoMap();

    ~EchoMap() noexcept override;

    /**
     * Runs the platform-dependent event loop to manage and propagate interaction with the EchoMap application.
     *
     * This function returns only once GLFW indicates that the window should close. Following closure, the event loop
     * could be re-run, or the application could clean up by calling the destructor.
     */
    virtual void run_event_loop() = 0;

    /**
     * Submit a new Notification to the application queue.
     *
     * Notifications are processed at the beginning of render cycles in a first-come first-served ordering.
     *
     * @param notification The Notification to schedule.
     */
    void notify(Notification&& notification) override;

    EchoMap(const EchoMap&) = delete;
    EchoMap& operator=(const EchoMap&) = delete;
    EchoMap(EchoMap&&) = delete;
    EchoMap& operator=(EchoMap&&) = delete;

protected:
    void tick();

    /**
     * Produce an overload set for @ref std::visit for all platform-independent Notification objects.
     *
     * @return The overload set.
     */
    auto make_common_notification_visitors()
    {
        // clang-format off
        return variant_helpers::Overloaded{
            [this](const AddChannelMappingNotification& n) { project_controller.handle_notification(n); },
            [this](const ModifySensorColourNotification& n) { project_controller.handle_notification(n); },
            [this](const ModifySensorPositionNotification& n) { project_controller.handle_notification(n); },
            [this](const ProjectSelectionCompleteNotification& n) { handle_notification(n); },
            [this](const ClearErrorNotification& n) { handle_notification(n); },
        };
        // clang-format on
    }

    /**
     * Uses @ref std::visit on the given notification to invoke the corresponding handler.
     *
     * This function is virtual, since the overload set can be platform-dependent in addition to the base handlers
     * provided by @ref make_common_notification_visitors.
     *
     * @param notification The notification to visit.
     */
    virtual void visit_notification(Notification notification) = 0;

    /**
     * Configure the core signals for the application instance.
     *
     * This should be invoked during construction prior to any IPanel invocations as it takes the exclusive consumer
     * role for several critical message classes.
     */
    void setup_subscriptions();

    /**
     * Handle any unconsumed Notification objects from the queue.
     */
    void process_notifications();

    /**
     * Handle any unconsumed events from the Worker.
     */
    void process_worker_results();

    void handle_notification(const ProjectSelectionCompleteNotification& notification);
    void handle_notification(const ClearErrorNotification& notification);

    // NOLINTBEGIN(*-non-private-member-variables-in-classes)

    Worker worker;                     /**< Multi-threaded worker for scheduling heavy computation tasks. */
    WorkerResultDespatcher despatcher; /**< Despatcher to manage Worker result channels. */
    std::vector<sigc::scoped_connection> connections;   /**< RAII lifetime manager for signal connections. */

    std::deque<Notification> notification_queue; /**< FIFO queue for Notification objects. */
    std::unique_ptr<Project> project;            /**< Owning container for the active Project. */

    RenderHost render_host;
    PanelHost panel_host;

#ifdef __EMSCRIPTEN__
    PartialProjectController project_controller;
#else
    FullProjectController project_controller;
#endif

    // NOLINTEND(*-non-private-member-variables-in-classes)
};

} // namespace echomap

#endif // ECHOMAP_ECHOMAP_HPP
