//
// Created by owd on 04/06/2026.
//

#include "ViewportRenderer.hpp"

#include "webgpu/webgpu_glfw.h"

namespace WebCFD
{

ViewportRenderer::ViewportRenderer(
        const wgpu::Device& device,
        const wgpu::TextureFormat texture_format,
        const std::uint32_t width,
        const std::uint32_t height) :
    width(width),
    height(height),
    device(device),
    texture_format(texture_format)
{
    install_shader();
    recreate_texture();
}

void ViewportRenderer::render(const wgpu::CommandEncoder &command_encoder) const
{
    if (!texture_view)
        return;

    wgpu::RenderPassColorAttachment attachment{
        .view = texture_view,
        .loadOp = wgpu::LoadOp::Clear,
        .storeOp = wgpu::StoreOp::Store,
        .clearValue = {0.02, 0.02, 0.04, 1.0}
    };

    const wgpu::RenderPassDescriptor pass_descriptor{
        .colorAttachmentCount = 1,
        .colorAttachments = &attachment
    };

    const wgpu::RenderPassEncoder pass =
        command_encoder.BeginRenderPass(&pass_descriptor);

    pass.SetPipeline(pipeline);
    pass.Draw(3);
    pass.End();
}

void ViewportRenderer::resize(const std::uint32_t new_width, const std::uint32_t new_height)
{
    if (new_width == 0 || new_height == 0)
        return;

    if (new_width == width && new_height == height)
        return;

    width = new_width;
    height = new_height;

    recreate_texture();
}

wgpu::TextureView ViewportRenderer::get_texture_view() const noexcept
{
    return texture_view;
}

void ViewportRenderer::install_shader()
{
    static constexpr char shader_code[] = R"(
        @vertex fn vertexMain(@builtin(vertex_index) i : u32) ->
          @builtin(position) vec4f {
            const pos = array(vec2f(0, 1), vec2f(-1, -1), vec2f(1, -1));
            return vec4f(pos[i], 0, 1);
        }
        @fragment fn fragmentMain() -> @location(0) vec4f {
            return vec4f(1, 0, 0, 1);
        }
    )";

    wgpu::ShaderSourceWGSL wgsl{{.code = shader_code}};
    const wgpu::ShaderModuleDescriptor module_descriptor{.nextInChain = &wgsl};
    const wgpu::ShaderModule module = device.CreateShaderModule(&module_descriptor);

    wgpu::ColorTargetState colour_target{.format = texture_format};
    wgpu::FragmentState fragment{
        .module = module,
        .targetCount = 1,
        .targets = &colour_target
    };

    const wgpu::RenderPipelineDescriptor pipeline_descriptor{
        .vertex = {.module = module},
        .fragment = &fragment
    };

    pipeline = device.CreateRenderPipeline(&pipeline_descriptor);
}

void ViewportRenderer::recreate_texture()
{
    wgpu::TextureDescriptor texture_descriptor{};
    texture_descriptor.dimension = wgpu::TextureDimension::e2D;
    texture_descriptor.size = {width, height, 1};
    texture_descriptor.format = texture_format;
    texture_descriptor.usage = wgpu::TextureUsage::RenderAttachment | wgpu::TextureUsage::TextureBinding;

    texture = device.CreateTexture(&texture_descriptor);
    texture_view = texture.CreateView();
}

} // WebCFD
