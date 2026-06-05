//
// Created by owd on 04/06/2026.
//

#ifndef WEBCFD_WEBCFD_HPP
#define WEBCFD_WEBCFD_HPP

#include <GLFW/glfw3.h>
#include <webgpu/webgpu_cpp.h>

#include "IPanel.hpp"

namespace WebCFD
{

class WebCFD
{
public:
    WebCFD();

    ~WebCFD();

private:
    static constexpr std::uint32_t default_width = 512;
    static constexpr std::uint32_t default_height = 512;
    static constexpr auto default_timeout = std::numeric_limits<std::uint64_t>::max();

    static GLFWwindow* create_window();

    static std::pair<
            wgpu::Surface,
            wgpu::TextureFormat>
    create_surface(
            const wgpu::Adapter& adapter,
            GLFWwindow* window,
            const wgpu::Instance& instance,
            const wgpu::Device& device
    );

    wgpu::Future request_adapter();
    wgpu::Future request_device();

    void run_event_loop();
    void render() const;
    void setup_gui();

#ifdef __EMSCRIPTEN__

    // ReSharper disable once CppParameterMayBeConstPtrOrRef - Function signature enforced by Emscripten API.
    /**
     * Invokes WebCFD::render from a static context given an untyped mutable pointer to the WebCFD object instance.
     *
     * @note This function provided when targeting Emscripten only. It is intended to be used as a function callback
     *  from the Emscripten C API.
     *
     * @param webcfd_instance The WebCFD application instance on which to invoke WebCFD::render.
     */
    static void render_shim(
            void* const webcfd_instance
    )
    {
        const auto* instance = static_cast<WebCFD*>(webcfd_instance);
        instance->render();
    }
#endif

    wgpu::Instance instance;
    wgpu::Adapter adapter;
    wgpu::Device device;
    wgpu::Surface surface;
    wgpu::TextureFormat default_format;
    GLFWwindow* window;

    bool swapchain_rebuild = false;
    std::vector<std::unique_ptr<IPanel>> panels;
};

} // namespace WebCFD

#endif // WEBCFD_WEBCFD_HPP
