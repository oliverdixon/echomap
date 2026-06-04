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

    static GLFWwindow *create_window();

    static std::pair<wgpu::Surface, wgpu::TextureFormat> create_surface(
        const wgpu::Adapter &adapter,
        GLFWwindow *window,
        const wgpu::Instance &instance,
        const wgpu::Device &device);

    wgpu::Future request_adapter();
    wgpu::Future request_device();

    void run_event_loop();
    void render() const;
    void setup_gui();

#ifdef __EMSCRIPTEN__
    // ReSharper disable once CppParameterMayBeConstPtrOrRef - Function signature enforced by Emscripten API.
    static void render_shim(void * const user_data)
    {
        auto* instance = static_cast<WebCFD*>(user_data);
        instance->render();
    }
#endif

    wgpu::Instance instance;
    wgpu::Adapter adapter;
    wgpu::Device device;
    wgpu::Surface surface;
    wgpu::TextureFormat default_format;
    GLFWwindow * window;

    std::vector<std::unique_ptr<IPanel>> panels;
};

} // WebGPU

#endif // WEBCFD_WEBCFD_HPP
