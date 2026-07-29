/**
 * @file
 *
 * RenderHost specification
 *
 * @author Oliver Dixon
 * @date 2026-07-28
 */

#ifndef ECHOMAP_RENDERHOST_HPP
#define ECHOMAP_RENDERHOST_HPP

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <webgpu/webgpu_cpp.h>

#include "IRenderInvalidator.hpp"

namespace echomap
{
class PanelHost;

/**
 * Provides services for low-level rendering.
 */
class RenderHost : public IRenderInvalidator
{
public:
    RenderHost();

    ~RenderHost() noexcept override;

    RenderHost(const RenderHost&) = delete;
    RenderHost& operator=(const RenderHost&) = delete;
    RenderHost(RenderHost&&) noexcept = delete;
    RenderHost& operator=(RenderHost&&) noexcept = delete;

    void force_frames(unsigned int count) noexcept override;

    void render(const PanelHost& panel_host);

    [[nodiscard]] bool wait_for_frame_trigger() const noexcept;

    void process_instance_events() const;

private:
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

    std::uint32_t viewport_width = 1024;
    std::uint32_t viewport_height = 1024;

    wgpu::Instance instance;
    wgpu::Adapter adapter;
    wgpu::Device device;
    wgpu::Surface surface;
    wgpu::SurfaceCapabilities surface_capabilities;
    GLFWwindow* window = nullptr;

    ImGuiID dockspace_id;
    bool dockspace_configured = false;
    unsigned int forced_frames = 0;
};

} // namespace echomap

#endif // ECHOMAP_RENDERHOST_HPP
