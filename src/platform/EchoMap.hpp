/**
 * @file
 * @brief EchoMap class specification
 * @author Oliver Dixon
 * @date 2026-05-05
 */

#ifndef ECHOMAP_ECHOMAP_HPP
#define ECHOMAP_ECHOMAP_HPP

#include <sigc++/scoped_connection.h>

#include "../async/Worker.hpp"
#include "../async/WorkerResultDespatcher.hpp"
#include "../controllers/PanelHost.hpp"
#include "../controllers/RenderHost.hpp"

/**
 * The main EchoMap outermost namespace for all non-exported symbols.
 */
namespace echomap
{

class ProjectControllerBase;

/**
 * The EchoMap maintains state for the application including WebGPU and Dear ImGui context, encapsulating
 * initialisation, game loop, interaction, and clean-up.
 */
class EchoMap
{
public:
    virtual ~EchoMap() noexcept;

    /**
     * Runs the platform-dependent event loop to manage and propagate interaction with the EchoMap application.
     *
     * This function returns only once GLFW indicates that the window should close. Following closure, the event loop
     * could be re-run, or the application could clean up by calling the destructor.
     */
    virtual void run_event_loop() = 0;

    EchoMap(const EchoMap&) = delete;
    EchoMap& operator=(const EchoMap&) = delete;
    EchoMap(EchoMap&&) = delete;
    EchoMap& operator=(EchoMap&&) = delete;

protected:
    /**
     * Initialise a EchoMap application instance.
     *
     * Initialisation is a computationally substantial task. Context from all managed frameworks must be initialised
     * (GLFW, WebGPU/Dawn, and Dear ImGui) and their components registered. Once the constructor has completed, the game
     * loop can begin with @ref run_event_loop.
     *
     * @throws ConfigurationError Some part of initialisation, described in the exception message, did not succeed.
     */
    EchoMap(std::function<void()> worker_result_callback = {});

    void setup_controller(std::unique_ptr<ProjectControllerBase> controller);

    /**
     * Handle any unconsumed events from the Worker.
     */
    void process_worker_results();

    // NOLINTBEGIN(*-non-private-member-variables-in-classes)

    Worker worker;                     /**< Multi-threaded worker for scheduling heavy computation tasks. */
    WorkerResultDespatcher despatcher; /**< Despatcher to manage Worker result channels. */
    std::vector<sigc::scoped_connection> connections;   /**< RAII lifetime manager for signal connections. */

    RenderHost render_host;
    PanelHost panel_host;
    std::unique_ptr<ProjectControllerBase> project_controller;

    // NOLINTEND(*-non-private-member-variables-in-classes)
};

} // namespace echomap

#endif // ECHOMAP_ECHOMAP_HPP
