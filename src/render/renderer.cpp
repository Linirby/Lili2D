#include "lili2d/render/renderer.hpp"

#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL_pixels.h>

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "lili2d/core/sdl_deleters.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/scene/common/model.hpp"
#include "lili2d/render/scene/common/utils.hpp"
#include "lili2d/render/scene/shapes/circle.hpp"
#include "lili2d/render/scene/shapes/rect.hpp"
#include "lili2d/render/white_1x1_png.hpp"
#include "shader/world_2d_frag_spv.hpp"
#include "shader/world_2d_vert_spv.hpp"
// #include "shader/ui_vert_spv.hpp"
// #include "shader/ui_frag_spv.hpp"

#include "lili2d/geometry/mat3x3.hpp"

namespace lili {

Renderer::Renderer(Window* window, SDL_GPUPresentMode preferred_mode)
    : window(window) {
    initDevice(preferred_mode);
    initShaders();
    initPipelines();
    initPasses();
    the_white_pixel = std::make_unique<Texture>(
        device.get(), white_1x1_png, white_1x1_png_len
    );
}

Renderer::~Renderer() {
    if (device) SDL_WaitForGPUIdle(device.get());
}

SDL_GPUDevice*
Renderer::getDevice() const {
    return device.get();
}

bool
Renderer::beginFrame() {
    current_cmd_buffer = SDL_AcquireGPUCommandBuffer(device.get());
    if (!current_cmd_buffer)
        throw std::runtime_error("Failed to acquire command buffer!");

    uint32_t width = 0;
    uint32_t height = 0;
    bool success = SDL_WaitAndAcquireGPUSwapchainTexture(
        current_cmd_buffer, window->getSdlWindow(), &current_swapchain_texture,
        &width, &height
    );
    if (!success)
        throw std::runtime_error("Failed to acquire swapchain texture!");
    if (!current_swapchain_texture) {
        SDL_SubmitGPUCommandBuffer(current_cmd_buffer);
        current_cmd_buffer = nullptr;
        return false;
    }

    if (width != swapchain_width || height != swapchain_height) {
        swapchain_width = width;
        swapchain_height = height;
    }

    Vec2 logical_res = getLogicalResolution();

    Mat3 projection =
        Mat3::orthographic(0.0f, logical_res.x, 0.0f, logical_res.y);
    Mat3 view = Mat3::identity();
    if (camera) {
        projection = camera->getProjection(logical_res.x, logical_res.y);
        view = camera->getViewMatrix(logical_res.x, logical_res.y);
    }
    proj_view_world2d = projection * view;

    Mat3 ui_translation = Mat3::translate({0.0f, 0.0f});
    Mat3 ui_rotation = Mat3::rotation(0.0f);
    Mat3 ui_scale = Mat3::scale({1.0f, 1.0f});
    Mat3 ui_view = ui_scale * ui_rotation * ui_translation;
    proj_view_ui = projection * ui_view;
    return true;
}

void
Renderer::submit(
    Model model, const Mat3& transform, float layer, RenderLayer layer_type
) {
    if (layer_type == RenderLayer::WORLD2D)
        world_2d_queue[layer].push_back({model, transform, layer});
    if (layer_type == RenderLayer::UI)
        ui_queue[layer].push_back({model, transform, layer});
    if (layer_type == RenderLayer::PIXEL_WORLD2D)
        pixel_world_2d_queue[layer].push_back({model, transform, layer});
}

void
Renderer::endFrame() {
    if (!current_cmd_buffer || !current_swapchain_texture) return;

    Vec2 logical_res = getLogicalResolution();
    if (logical_res.x > 0.0f && logical_res.y > 0.0f) {
        float scale_x = static_cast<float>(swapchain_width) / logical_res.x;
        float scale_y = static_cast<float>(swapchain_height) / logical_res.y;
        float scale = std::min(scale_x, scale_y);

        viewport_w = logical_res.x * scale;
        viewport_h = logical_res.y * scale;
        viewport_x = (static_cast<float>(swapchain_width) - viewport_w) / 2.0f;
        viewport_y = (static_cast<float>(swapchain_height) - viewport_h) / 2.0f;
    }

    offscreenRender();

    SDL_GPUBlitRegion src{};
    src.texture = current_offscreen_texture.get();
    src.x = 0;
    src.y = 0;
    src.w = offscreen_width;
    src.h = offscreen_height;

    SDL_GPUBlitRegion dst{};
    dst.texture = current_swapchain_texture;
    dst.x = viewport_x;
    dst.y = viewport_y;
    dst.w = viewport_w;
    dst.h = viewport_h;

    SDL_GPUBlitInfo blit_info{};
    blit_info.source = src;
    blit_info.destination = dst;
    blit_info.load_op = SDL_GPU_LOADOP_CLEAR;
    blit_info.clear_color = SDL_FColor{0.0f, 0.0f, 0.0f, 1.0f};
    blit_info.filter = SDL_GPU_FILTER_NEAREST;

    SDL_BlitGPUTexture(current_cmd_buffer, &blit_info);

    swapchainRender();

    world_2d_queue.clear();
    ui_queue.clear();
    pixel_world_2d_queue.clear();
    SDL_SubmitGPUCommandBuffer(current_cmd_buffer);
    current_cmd_buffer = nullptr;
}

void
Renderer::setCamera(Camera* camera) {
    this->camera = camera;
}

Camera*
Renderer::getCamera() const {
    return camera;
}

uint32_t
Renderer::getSwapchainWidth() const {
    return swapchain_width;
}

uint32_t
Renderer::getSwapchainHeight() const {
    return swapchain_height;
}

void
Renderer::setPresentMode(SDL_GPUPresentMode mode) {
    if (mode == SDL_GPU_PRESENTMODE_MAILBOX) {
        if (!SDL_SetGPUSwapchainParameters(
                device.get(), window->getSdlWindow(),
                SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_MAILBOX
            )) {
            std::cout << "Mailbox unavailable, trying Immediate\n";
            if (!SDL_SetGPUSwapchainParameters(
                    device.get(), window->getSdlWindow(),
                    SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
                    SDL_GPU_PRESENTMODE_IMMEDIATE
                )) {
                std::cout << "Immediate unavailable, trying VSync\n";
                SDL_SetGPUSwapchainParameters(
                    device.get(), window->getSdlWindow(),
                    SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_VSYNC
                );
            }
        }
    } else if (mode == SDL_GPU_PRESENTMODE_IMMEDIATE) {
        if (!SDL_SetGPUSwapchainParameters(
                device.get(), window->getSdlWindow(),
                SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_IMMEDIATE
            )) {
            std::cout << "Immediate unavailable, trying VSync\n";
            SDL_SetGPUSwapchainParameters(
                device.get(), window->getSdlWindow(),
                SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_VSYNC
            );
        }
    } else
        SDL_SetGPUSwapchainParameters(
            device.get(), window->getSdlWindow(),
            SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_VSYNC
        );
}

void
Renderer::setLogicalResolution(int width, int height) {
    logical_width = width;
    logical_height = height;
}

Vec2
Renderer::getLogicalResolution() const {
    if (logical_width > 0 && logical_height > 0)
        return {
            static_cast<float>(logical_width),
            static_cast<float>(logical_height)
        };
    if (window && window->hasLogicalResolution())
        return window->getLogicalResolution();
    return {
        static_cast<float>(swapchain_width),
        static_cast<float>(swapchain_height)
    };
}

Shader*
Renderer::createShader(
    const std::string& vert_path, const std::string& frag_path,
    ShaderInfo vert_infos, ShaderInfo frag_infos
) {
    return new Shader(
        device.get(), vert_path, frag_path, vert_infos, frag_infos
    );
}

Shader*
Renderer::createShader(
    const uint8_t* vert_code, size_t vert_size, const uint8_t* frag_code,
    size_t frag_size, ShaderInfo vert_infos, ShaderInfo frag_infos
) {
    return new Shader(
        device.get(), vert_code, vert_size, frag_code, frag_size, vert_infos,
        frag_infos
    );
}

MainGraphicsPipeline*
Renderer::createMainGraphicsPipeline(Shader* shader) {
    return new MainGraphicsPipeline(
        device.get(), window->getSdlWindow(), shader
    );
}

Texture*
Renderer::getTheWhitePixel() const {
    return the_white_pixel.get();
}

GPUMesh*
Renderer::getUnitQuad() {
    if (!unit_quad)
        unit_quad = std::make_unique<GPUMesh>(device.get(), createUnitQuad());
    return unit_quad.get();
}

GPUMesh*
Renderer::getUnitCircle(int segments) {
    if (unit_circles.find(segments) == unit_circles.end())
        unit_circles[segments] =
            std::make_unique<GPUMesh>(device.get(), createUnitCircle(segments));
    return unit_circles[segments].get();
}

void
Renderer::drawDebugRect(float x, float y, float w, float h, const Vec4& color) {
    uint32_t r = (uint32_t)(color.x * 255.0f);
    uint32_t g = (uint32_t)(color.y * 255.0f);
    uint32_t b = (uint32_t)(color.z * 255.0f);
    uint32_t a = (uint32_t)(color.w * 255.0f);
    uint32_t key = (r << 24) | (g << 16) | (b << 8) | a;

    if (debug_rects.find(key) == debug_rects.end()) {
        debug_rects[key] = std::make_unique<Rect>(this, RectShape(), color);
        debug_rects[key]->setHollow(true);
    }

    debug_rects[key]->setShape(RectShape(x, y, w, h));
    debug_rects[key]->draw();
}

void
Renderer::drawDebugCircle(float x, float y, float radius, const Vec4& color) {
    uint32_t r = (uint32_t)(color.x * 255.0f);
    uint32_t g = (uint32_t)(color.y * 255.0f);
    uint32_t b = (uint32_t)(color.z * 255.0f);
    uint32_t a = (uint32_t)(color.w * 255.0f);
    uint32_t key = (r << 24) | (g << 16) | (b << 8) | a;

    if (debug_circles.find(key) == debug_circles.end()) {
        debug_circles[key] =
            std::make_unique<Circle>(this, CircleShape(), color);
        debug_circles[key]->setHollow(true);
    }

    debug_circles[key]->setShape(CircleShape(Vec2(x, y), radius, 16));
    debug_circles[key]->draw();
}

void
Renderer::initDevice(SDL_GPUPresentMode preferred_mode) {
    device = std::unique_ptr<SDL_GPUDevice, SDLGPUDeviceDeleter>(
        SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr)
    );
    if (!device)
        throw std::runtime_error(
            "Device creation failed!\n-> " + std::string(SDL_GetError())
        );
    if (!SDL_ClaimWindowForGPUDevice(device.get(), window->getSdlWindow()))
        throw std::runtime_error(
            "SDL_ClaimWindowForGPUDevice() failed!\n-> " +
            std::string(SDL_GetError())
        );
    setPresentMode(preferred_mode);
}

void
Renderer::initShaders() {
    main_shader = std::make_unique<Shader>(
        device.get(), world_2d_vert_spv, world_2d_vert_spv_len,
        world_2d_frag_spv, world_2d_frag_spv_len,
        ShaderInfo({.num_uniform_buffers = 1}),
        ShaderInfo({
            .num_samplers = 1,
        })
    );
}

void
Renderer::initPipelines() {
    main_pipeline = std::make_unique<MainGraphicsPipeline>(
        device.get(), window->getSdlWindow(), main_shader.get()
    );
}

void
Renderer::initPasses() {
    render_pass = std::make_unique<MainRenderPass>(
        device.get(), main_pipeline->getSdlPipeline()
    );
}

void
Renderer::offscreenRender() {
    Vec2 logical_res = getLogicalResolution();
    uint32_t target_w = static_cast<uint32_t>(logical_res.x);
    uint32_t target_h = static_cast<uint32_t>(logical_res.y);

    bool not_same_size =
        target_w != offscreen_width || target_h != offscreen_height;
    if (!current_offscreen_texture || not_same_size) {
        if (current_offscreen_texture) current_offscreen_texture.reset(nullptr);

        SDL_GPUTextureCreateInfo ci{};
        ci.type = SDL_GPU_TEXTURETYPE_2D;
        ci.format = SDL_GetGPUSwapchainTextureFormat(
            device.get(), window->getSdlWindow()
        );
        ci.usage =
            SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
        ci.width = target_w;
        ci.height = target_h;
        ci.layer_count_or_depth = 1;
        ci.num_levels = 1;

        current_offscreen_texture =
            std::unique_ptr<SDL_GPUTexture, SDLGPUTextureDeleter>(
                SDL_CreateGPUTexture(device.get(), &ci),
                SDLGPUTextureDeleter(device.get())
            );
        offscreen_width = target_w;
        offscreen_height = target_h;
    }

    SDL_GPUColorTargetInfo color_ti{};
    color_ti.texture = current_offscreen_texture.get();
    color_ti.clear_color = SDL_FColor{0.0f, 0.0f, 0.0f, 1.0f};
    color_ti.load_op = SDL_GPU_LOADOP_CLEAR;
    color_ti.store_op = SDL_GPU_STOREOP_STORE;

    SDL_GPURenderPass* pass =
        SDL_BeginGPURenderPass(current_cmd_buffer, &color_ti, 1, nullptr);

    SDL_GPUViewport vp{
        0.0f, 0.0f, static_cast<float>(target_w), static_cast<float>(target_h),
        0.0f, 1.0f
    };
    SDL_SetGPUViewport(pass, &vp);

    render_pass->render(
        pass, current_cmd_buffer, proj_view_world2d, pixel_world_2d_queue
    );

    SDL_EndGPURenderPass(pass);
}

void
Renderer::swapchainRender() {
    SDL_GPUColorTargetInfo color_ti{};
    color_ti.texture = current_swapchain_texture;
    color_ti.load_op = SDL_GPU_LOADOP_LOAD;
    color_ti.store_op = SDL_GPU_STOREOP_STORE;

    SDL_GPURenderPass* pass =
        SDL_BeginGPURenderPass(current_cmd_buffer, &color_ti, 1, nullptr);

    SDL_GPUViewport vp{viewport_x, viewport_y, viewport_w,
                       viewport_h, 0.0f,       1.0f};
    SDL_SetGPUViewport(pass, &vp);

    render_pass->render(
        pass, current_cmd_buffer, proj_view_world2d, world_2d_queue
    );
    render_pass->render(pass, current_cmd_buffer, proj_view_ui, ui_queue);

    SDL_EndGPURenderPass(pass);
}

}  // namespace lili
