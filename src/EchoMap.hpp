/**
 * @file
 * @brief EchoMap class specification
 * @author Oliver Dixon
 * @date 2026-05-05
 */

#ifndef ECHOMAP_ECHOMAP_HPP
#define ECHOMAP_ECHOMAP_HPP

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <sigc++/scoped_connection.h>
#include <webgpu/webgpu_cpp.h>

#include "notifications/AllNotificationsFwd.hpp"
#include "panels/ErrorModal.hpp"
#include "signals/Worker.hpp"
#include "signals/WorkerResultDespatcher.hpp"

/**
 * The main EchoMap outermost namespace for all non-exported symbols.
 */
namespace echomap
{

class IPanel;
class Project;

/**
 * The EchoMap maintains state for the application including WebGPU and Dear ImGui context, encapsulating
 * initialisation, game loop, interaction, and clean-up.
 */
class EchoMap
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

    /**
     * Runs the platform-dependent event loop to manage and propagate interaction with the EchoMap application.
     *
     * This function returns only once GLFW indicates that the window should close. Following closure, the event loop
     * could be re-run, or the application could clean up by calling the destructor.
     */
    virtual void run_event_loop() = 0;

    /**
     * Clean up all persistent state registered by the application instance.
     */
    virtual ~EchoMap() noexcept;

    void change_active_project(std::unique_ptr<Project> new_project) noexcept;

    /**
     * Submit a new Notification to the application queue.
     *
     * Notifications are processed at the beginning of render cycles in a first-come first-served ordering.
     *
     * @param notification The Notification to schedule.
     */
    void notify(const Notification& notification);

    /**
     * Indicate to the renderer that the following frames should always be rendered, regardless of whether there are any
     * new events to process.
     *
     * The forced frame count stacks (as in a source) until the renderer drains it to zero (as in a sink). By default,
     * we force four frames since most Dear ImGui components can fully render a four-frame cycle.
     *
     * @param count The number of frames to force.
     */
    void increment_forced_frames(unsigned int count = 4) noexcept;

    EchoMap(const EchoMap&) = delete;
    EchoMap& operator=(const EchoMap&) = delete;
    EchoMap(EchoMap&&) = delete;
    EchoMap& operator=(EchoMap&&) = delete;

protected:
    /**
     * Produce an overload set for @ref std::visit for all platform-independent Notification objects.
     *
     * @return The overload set.
     */
    auto make_common_notification_visitors()
    {
        // clang-format off
        return variant_helpers::Overloaded{
            [this](const AddChannelMappingNotification& n) { handle_notification(n); },
            [this](const ModifySensorColourNotification& n) { handle_notification(n); },
            [this](const ModifySensorPositionNotification& n) { handle_notification(n); },
            [this](const ProjectSelectionCompleteNotification& n) { handle_notification(n); },
            [this](const CompleteProjectLoadNotification& n) { handle_notification(n); },
            [this](RegisterVFSMappingNotification& n) { handle_notification(n); },
            [this](const CancelProjectLoadNotification& n) { handle_notification(n); },
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
    virtual void visit_notification(Notification& notification) = 0;

    /**
     * Perform a render cycle on the configured Surface and Device.
     *
     * A single render cycle requests all panels to render their state to the Surface, and provides an opportunity to
     * submit any work to the GPU. Events are also received from GLFW and processed as required.
     */
    void render() noexcept;

    static constexpr auto operation_timeout = std::numeric_limits<std::uint64_t>::max();

    /**
     * Create a new GLFW window of the specified dimensions from a static context.
     *
     * @param width Initial width of the window, in pixels.
     * @param height Initial height of the window, in pixels.
     * @return A mutable pointer to the created window, which must be explicitly deleted following use.
     * @throws ConfigurationError A GLFW initialisation step failed.
     */
    static GLFWwindow* create_window(
            int width,
            int height
    );

    /**
     * Configure a WebGPU Surface from a static context given metadata and Adapter capabilities.
     *
     * @note There is no way to check if the given Surface is already configured from the public WebGPU.h API. Moreover,
     *  attempting to Unconfigure an unconfigured Surface will assert. Therefore, callers must ensure that the given
     *  Surface is in an unconfigured state prior to invoking this function, as it cannot sanity-check the state of the
     *  Surface.
     *
     * @param surface The Surface to configure.
     * @param device The WebGPU Device on which the Surface will be displayed.
     * @param capabilities Capabilities of the WebGPU Adapter and Instance.
     * @param viewport_width Initial width of the Surface viewport, in pixels.
     * @param viewport_height Initial height of the Surface viewport, in pixels.
     */
    static void configure_surface(
            const wgpu::Surface& surface,
            const wgpu::Device& device,
            const wgpu::SurfaceCapabilities& capabilities,
            std::uint32_t viewport_width,
            std::uint32_t viewport_height
    ) noexcept;

    /**
     * Configure the core signals for the application instance.
     *
     * This should be invoked during construction prior to any IPanel invocations as it takes the exclusive consumer
     * role for several critical message classes.
     */
    void setup_subscriptions();

    /**
     * Produce a WebGPU Future for requesting an Adapter.
     *
     * @return A Future to request an Adapter that is suitable for the Surface member from the WebGPU driver.
     */
    wgpu::Future request_adapter() noexcept;

    /**
     * Produce a WebGPU Future for requesting an accelerator device.
     *
     * @return A Future to request a Device from the WebGPU driver.
     */
    wgpu::Future request_device() noexcept;

    /**
     * Create a context for Dear ImGui and ImPlot, and configure the plain GLFW and WebGPU backends.
     *
     * @throws ConfigurationError A Dear ImGui backend could not be initialised.
     */
    void setup_imgui();

    void setup_dockspace();

    /**
     * Check if the window has been resized compared with the stored dimensions, updating member variables and
     * reconfiguring the WebGPU surface if necessary.
     *
     * @return Is the window visible?
     */
    bool handle_window_resize() noexcept;

    /**
     * Handle any unconsumed Notification objects from the queue.
     */
    void process_notifications();

    /**
     * Handle any unconsumed events from the Worker.
     */
    void process_worker_results();

    void raise_error(std::string_view message);
    void raise_error(
            std::string_view message,
            const std::runtime_error& exception
    );

    void handle_notification(const AddChannelMappingNotification& notification) const;
    void handle_notification(const ModifySensorColourNotification& notification) const;
    void handle_notification(const ModifySensorPositionNotification& notification) const;
    void handle_notification(const ProjectSelectionCompleteNotification& notification);
    void handle_notification(const CompleteProjectLoadNotification& notification);
    void handle_notification(RegisterVFSMappingNotification& notification) const;
    void handle_notification(const CancelProjectLoadNotification& notification);
    void handle_notification(const ClearErrorNotification& notification);

    void handle_result(LoadProjectResult&& result);
    void handle_result(LoadSignalFileResult&& result);

    // NOLINTBEGIN(*-non-private-member-variables-in-classes)

    std::uint32_t viewport_width = 1024;
    std::uint32_t viewport_height = 1024;

    wgpu::Instance instance;
    wgpu::Adapter adapter;
    wgpu::Device device;
    wgpu::Surface surface;
    wgpu::SurfaceCapabilities surface_capabilities;
    GLFWwindow* window = nullptr;

    Worker worker;                     /**< Multi-threaded worker for scheduling heavy computation tasks. */
    WorkerResultDespatcher despatcher; /**< Despatcher to manage Worker result channels. */
    std::vector<sigc::scoped_connection> connections; /**< RAII lifetime manager for signal connections. */

    std::vector<std::unique_ptr<IPanel>> panels;      /**< Individual display components. */
    std::optional<ErrorModal> error_modal;            /**< Persistent panel to indicate errors over all other panels. */
    std::vector<Notification> notification_queue;
    std::unique_ptr<Project> project;          /**< Owning container for the active Project. */
    std::unique_ptr<Project> unloaded_project; /**< Owning container for the unloaded Project. */
    std::unique_ptr<IPanel> active_modal;      /**< The current active non-ErrorModal modal panel. */

    ImGuiID dockspace_id;
    bool dockspace_configured = false;
    unsigned int forced_frames = 0;

    // NOLINTEND(*-non-private-member-variables-in-classes)
};

} // namespace echomap

#endif // ECHOMAP_ECHOMAP_HPP
