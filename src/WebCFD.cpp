//
// Created by owd on 04/06/2026.
//

#include <iostream>

#include <imgui.h>
#include <dawn/webgpu_cpp_print.h>
#include <webgpu/webgpu_glfw.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_wgpu.h>

#include "ParametersPanel.hpp"
#include "RenderPanel.hpp"
#if defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#endif

#include "WebCFD.hpp"

namespace WebCFD
{
WebCFD::WebCFD()
{
    static constexpr auto timed_wait_any = wgpu::InstanceFeatureName::TimedWaitAny;
    constexpr wgpu::InstanceDescriptor instance_desc{
        .requiredFeatureCount = 1,
        .requiredFeatures = &timed_wait_any
    };

    instance = wgpu::CreateInstance(&instance_desc);
    window = create_window();

    instance.WaitAny(request_adapter(), default_timeout);
    instance.WaitAny(request_device(), default_timeout);

    std::tie(surface, default_format) = create_surface(adapter, window, instance, device);

    setup_gui();
    run_event_loop();
}

WebCFD::~WebCFD()
{
    for (auto& panel : panels)
        panel.reset();

    if (ImGui::GetCurrentContext()) {
        ImGui_ImplWGPU_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    if (surface) {
        surface.Unconfigure();
        surface = nullptr;
    }

    device = nullptr;
    adapter = nullptr;
    instance = nullptr;

    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
}

void WebCFD::run_event_loop()
{
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(&WebCFD::render_shim, this, 0, true);
#else
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        render();
        // ReSharper disable once CppExpressionWithoutSideEffects
        surface.Present();
        instance.ProcessEvents();
    }
#endif
}

GLFWwindow *WebCFD::create_window()
{
    if (!glfwInit())
        throw std::runtime_error("glfwInit failed");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    const auto window = glfwCreateWindow(
        default_width,
        default_height,
        "WebCFD",
        nullptr,
        nullptr
    );

    if (!window) {
        glfwTerminate();
        throw std::runtime_error("glfwCreateWindow failed");
    }

    return window;
}

wgpu::Future WebCFD::request_adapter()
{
    const wgpu::RequestAdapterOptions options{
        .compatibleSurface = surface
    };

    return instance.RequestAdapter(
        &options, wgpu::CallbackMode::WaitAnyOnly,
        [this](const wgpu::RequestAdapterStatus status, wgpu::Adapter new_adapter, const wgpu::StringView message)
        {
            if (status != wgpu::RequestAdapterStatus::Success) {
                std::cout << "RequestAdapter: " << message << "\n";
                exit(0);
            }

            adapter = std::move(new_adapter);
        });
}

wgpu::Future WebCFD::request_device()
{
    wgpu::DeviceDescriptor desc{};
    desc.SetDeviceLostCallback(
        wgpu::CallbackMode::AllowSpontaneous,
        [](const wgpu::Device &, const wgpu::DeviceLostReason reason, const wgpu::StringView message)
        {
            if (reason == wgpu::DeviceLostReason::Destroyed)
                return;

            std::cout << "Device lost: " << reason << " - message: " << message << "\n";
        }
    );

    desc.SetUncapturedErrorCallback(
        [](const wgpu::Device &, const wgpu::ErrorType errorType, const wgpu::StringView message)
        {
            std::cout << "Error: " << errorType << " - message: " << message << "\n";
        }
    );

    return adapter.RequestDevice(
        &desc, wgpu::CallbackMode::WaitAnyOnly,
        [this](const wgpu::RequestDeviceStatus status, wgpu::Device new_device, const wgpu::StringView message)
        {
            if (status != wgpu::RequestDeviceStatus::Success) {
                std::cout << "RequestDevice: " << message << "\n";
                exit(0);
            }

            device = std::move(new_device);
        }
    );
}

std::pair<wgpu::Surface, wgpu::TextureFormat> WebCFD::create_surface(
    const wgpu::Adapter &adapter,
    GLFWwindow *const window,
    const wgpu::Instance &instance,
    const wgpu::Device &device)
{
    const auto surface = wgpu::glfw::CreateSurfaceForWindow(instance, window);
    wgpu::SurfaceCapabilities capabilities;
    surface.GetCapabilities(adapter, &capabilities);

    const wgpu::SurfaceConfiguration config{
        .device = device,
        .format = capabilities.formats[0],
        .usage = wgpu::TextureUsage::RenderAttachment,
        .width = default_width,
        .height = default_height,
        .alphaMode = capabilities.alphaModes[0],
        .presentMode = wgpu::PresentMode::Fifo
    };

    surface.Configure(&config);
    return {surface, capabilities.formats[0]};
}

void WebCFD::render() const
{
    ImGui_ImplWGPU_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    for (const auto &panel: panels)
        panel->draw();

    ImGui::Render();
    const wgpu::CommandEncoder encoder = device.CreateCommandEncoder();

    for (const auto &panel: panels)
        panel->update_gpu(encoder);

    wgpu::SurfaceTexture surface_texture{};
    surface.GetCurrentTexture(&surface_texture);

    const wgpu::TextureView surface_view = surface_texture.texture.CreateView();

    wgpu::RenderPassColorAttachment attachment{
        .view = surface_view,
        .loadOp = wgpu::LoadOp::Clear,
        .storeOp = wgpu::StoreOp::Store,
        .clearValue = {0.1, 0.1, 0.1, 1.0}
    };

    const wgpu::RenderPassDescriptor pass_descriptor{
        .colorAttachmentCount = 1,
        .colorAttachments = &attachment
    };

    const wgpu::RenderPassEncoder pass = encoder.BeginRenderPass(&pass_descriptor);
    ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), pass.Get());

    pass.End();

    const wgpu::CommandBuffer commands = encoder.Finish();
    device.GetQueue().Submit(1, &commands);
}

// ReSharper disable once CppMemberFunctionMayBeConst - Not semantically constant
void WebCFD::setup_gui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    if (!ImGui_ImplGlfw_InitForOther(window, true))
        throw std::runtime_error("ImGui_ImplGlfw_InitForOther failed");

    ImGui_ImplWGPU_InitInfo init_info{};
    init_info.Device = device.Get();
    init_info.RenderTargetFormat = static_cast<WGPUTextureFormat>(std::to_underlying(default_format));

    if (!ImGui_ImplWGPU_Init(&init_info))
        throw std::runtime_error("ImGui_ImplWGPU_Init failed");

    panels.emplace_back(std::make_unique<ParametersPanel>());

    // TODO: rendered shouldn't use surface texture format. It can just use a normal one such as Unorm8.
    panels.emplace_back(std::make_unique<RenderPanel>(device, default_format, default_width, default_height));
}

} // WebCFD
