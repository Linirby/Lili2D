#include "lili2d/render/renderer.hpp"

#include <SDL3/SDL_pixels.h>

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/2d/circle.hpp"
#include "lili2d/render/2d/line.hpp"
#include "lili2d/render/2d/rect.hpp"
#include "lili2d/render/2d/text.hpp"
#include "lili2d/render/common/model.hpp"
#include "lili2d/render/common/unit_mesh.hpp"
#include "lili2d/render/default_font.hpp"
#include "lili2d/render/default_shaders.hpp"
#include "lili2d/render/gpu/pass_types.hpp"

namespace lili {

struct TextKey
{
    std::string text;
    uint32_t color = 0;
    const BitmapFont* font = nullptr;

    bool
    operator==(const TextKey& other) const noexcept
    {
        return color == other.color && font == other.font && text == other.text;
    }
};

struct TextKeyHash
{
    std::size_t
    operator()(const TextKey& k) const noexcept
    {
        std::size_t h1 = std::hash<std::string>{}(k.text);
        std::size_t h2 = std::hash<uint32_t>{}(k.color);
        std::size_t h3 =
            std::hash<const void*>{}(static_cast<const void*>(k.font));
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

struct CachedTextEntry
{
    TextKey key;
    std::unique_ptr<Text> text_obj;
    uint64_t last_frame_used = 0;
    uint64_t last_used_counter = 0;
};

struct ShapesCache
{
    std::map<uint64_t, std::unique_ptr<Rect>> rects;
    std::map<uint64_t, std::unique_ptr<Circle>> circles;
    std::map<uint32_t, std::unique_ptr<Line>> lines;

    std::unique_ptr<BitmapFont> default_font;
    BitmapFont* custom_default_font = nullptr;

    static constexpr size_t MAX_TEXT_CACHE_SIZE = 512;
    uint64_t usage_counter = 0;
    uint64_t current_frame = 0;

    std::unordered_map<TextKey, size_t, TextKeyHash> text_map;
    std::vector<CachedTextEntry> text_entries;
    std::unordered_map<std::string, std::unique_ptr<Text>> id_texts;
};

Renderer::Renderer(Window* window, SDL_GPUPresentMode preferred_mode)
  : window(window)
  , shapes_cache(std::make_unique<ShapesCache>())
{
    initDevice(preferred_mode);
    initShaders();
    initPipelines();
    initPasses();
    const uint32_t white_pixel = 0xFFFFFFFF;
    auto temp_surf =
        std::unique_ptr<SDL_Surface, SDLSurfaceDeleter>(SDL_CreateSurfaceFrom(
            1, 1, SDL_PIXELFORMAT_RGBA32, const_cast<uint32_t*>(&white_pixel), 4
        ));
    the_white_pixel = std::make_unique<Texture>(device.get(), temp_surf.get());
}

Renderer::~Renderer()
{
    shapes_cache.reset();
    if (device)
        SDL_WaitForGPUIdle(device.get());
}

Renderer::Renderer(Renderer&& other) noexcept = default;
Renderer&
Renderer::operator=(Renderer&& other) noexcept = default;

bool
Renderer::beginFrame()
{
    if (shapes_cache)
        shapes_cache->current_frame++;
    current_cmd_buffer = SDL_AcquireGPUCommandBuffer(device.get());
    if (!current_cmd_buffer)
        throw std::runtime_error("Failed to acquire command buffer!");

    uint32_t width = 0;
    uint32_t height = 0;
    bool success = SDL_WaitAndAcquireGPUSwapchainTexture(
        current_cmd_buffer,
        window->getSdlWindow(),
        &current_swapchain_texture,
        &width,
        &height
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

    Mat3 ui_projection =
        Mat3::orthographic(0.0f, logical_res.x, 0.0f, logical_res.y);
    Mat3 projection = ui_projection;
    Mat3 view = Mat3::identity();
    if (camera) {
        projection = camera->getProjection(logical_res.x, logical_res.y);
        view = camera->getViewMatrix(logical_res.x, logical_res.y);
    }
    proj_view_world2d = projection * view;

    Mat3 ui_translation = Mat3::translate({ 0.0f, 0.0f });
    Mat3 ui_rotation = Mat3::rotation(0.0f);
    Mat3 ui_scale = Mat3::scale({ 1.0f, 1.0f });
    Mat3 ui_view = ui_scale * ui_rotation * ui_translation;
    proj_view_ui = ui_projection * ui_view;
    return true;
}

void
Renderer::submit(
    Model model,
    const Mat3& transform,
    float layer,
    RenderLayer layer_type
)
{
    if (layer_type == RenderLayer::WORLD2D)
        world_2d_queue[layer].push_back({ model, transform, layer });
    if (layer_type == RenderLayer::UI)
        ui_queue[layer].push_back({ model, transform, layer });
    if (layer_type == RenderLayer::PIXEL_WORLD2D)
        pixel_world_2d_queue[layer].push_back({ model, transform, layer });
}

void
Renderer::endFrame()
{
    if (!current_cmd_buffer || !current_swapchain_texture)
        return;

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
    blit_info.clear_color = SDL_FColor(0.0f, 0.0f, 0.0f, 1.0f);
    blit_info.filter = SDL_GPU_FILTER_NEAREST;

    SDL_BlitGPUTexture(current_cmd_buffer, &blit_info);

    swapchainRender();

    world_2d_queue.clear();
    ui_queue.clear();
    pixel_world_2d_queue.clear();
    SDL_SubmitGPUCommandBuffer(current_cmd_buffer);
    current_cmd_buffer = nullptr;

    if (shapes_cache && shapes_cache->current_frame % 180 == 0 &&
        shapes_cache->text_entries.size() > 64) {
        std::vector<CachedTextEntry> kept;
        kept.reserve(shapes_cache->text_entries.size());
        shapes_cache->text_map.clear();
        for (auto& entry : shapes_cache->text_entries) {
            if (shapes_cache->current_frame - entry.last_frame_used <= 300) {
                size_t new_idx = kept.size();
                shapes_cache->text_map[entry.key] = new_idx;
                kept.push_back(std::move(entry));
            }
        }
        shapes_cache->text_entries = std::move(kept);
    }
}

void
Renderer::setPresentMode(SDL_GPUPresentMode mode)
{
    if (mode == SDL_GPU_PRESENTMODE_MAILBOX) {
        if (!SDL_SetGPUSwapchainParameters(
                device.get(),
                window->getSdlWindow(),
                SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
                SDL_GPU_PRESENTMODE_MAILBOX
            )) {
            std::cout << "Mailbox unavailable, trying Immediate\n";
            if (!SDL_SetGPUSwapchainParameters(
                    device.get(),
                    window->getSdlWindow(),
                    SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
                    SDL_GPU_PRESENTMODE_IMMEDIATE
                )) {
                std::cout << "Immediate unavailable, trying VSync\n";
                SDL_SetGPUSwapchainParameters(
                    device.get(),
                    window->getSdlWindow(),
                    SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
                    SDL_GPU_PRESENTMODE_VSYNC
                );
            }
        }
    } else if (mode == SDL_GPU_PRESENTMODE_IMMEDIATE) {
        if (!SDL_SetGPUSwapchainParameters(
                device.get(),
                window->getSdlWindow(),
                SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
                SDL_GPU_PRESENTMODE_IMMEDIATE
            )) {
            std::cout << "Immediate unavailable, trying VSync\n";
            SDL_SetGPUSwapchainParameters(
                device.get(),
                window->getSdlWindow(),
                SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
                SDL_GPU_PRESENTMODE_VSYNC
            );
        }
    } else
        SDL_SetGPUSwapchainParameters(
            device.get(),
            window->getSdlWindow(),
            SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
            SDL_GPU_PRESENTMODE_VSYNC
        );
}

Vec2
Renderer::getLogicalResolution() const
{
    if (logical_width > 0 && logical_height > 0)
        return { static_cast<float>(logical_width),
                 static_cast<float>(logical_height) };
    if (window && window->hasLogicalResolution())
        return window->getLogicalResolution();
    return { static_cast<float>(swapchain_width),
             static_cast<float>(swapchain_height) };
}

Shader*
Renderer::createShader(
    const std::string& vert_path,
    const std::string& frag_path,
    const std::string& vert_entry,
    const std::string& frag_entry
)
{
    return new Shader(
        device.get(), vert_path, frag_path, vert_entry, frag_entry
    );
}

Shader*
Renderer::createShader(
    std::string_view vert_source,
    std::string_view frag_source,
    const std::string& vert_entry,
    const std::string& frag_entry
)
{
    std::unique_ptr<Shader> unique_shader = Shader::fromSource(
        device.get(), vert_source, frag_source, vert_entry, frag_entry
    );
    return unique_shader.release();
}

MainGraphicsPipeline*
Renderer::createMainGraphicsPipeline(Shader* shader)
{
    return new MainGraphicsPipeline(
        device.get(), window->getSdlWindow(), shader
    );
}

GPUMesh*
Renderer::getUnitQuad()
{
    if (!unit_quad)
        unit_quad = std::make_unique<GPUMesh>(device.get(), createUnitQuad());
    return unit_quad.get();
}

GPUMesh*
Renderer::getUnitCircle(int segments)
{
    if (unit_circles.find(segments) == unit_circles.end())
        unit_circles[segments] =
            std::make_unique<GPUMesh>(device.get(), createUnitCircle(segments));
    return unit_circles[segments].get();
}

namespace {

inline uint32_t
colorToKey(const Vec4& color) noexcept
{
    uint32_t r = static_cast<uint32_t>(color.x * 255.0f);
    uint32_t g = static_cast<uint32_t>(color.y * 255.0f);
    uint32_t b = static_cast<uint32_t>(color.z * 255.0f);
    uint32_t a = static_cast<uint32_t>(color.w * 255.0f);
    return (r << 24) | (g << 16) | (b << 8) | a;
}

inline uint64_t
shapeKey(const Vec4& color, bool hollow) noexcept
{
    return (static_cast<uint64_t>(hollow ? 1 : 0) << 32) |
           static_cast<uint64_t>(colorToKey(color));
}

} // namespace

void
Renderer::drawRect(
    float x,
    float y,
    float w,
    float h,
    Vec4 color,
    bool hollow,
    RenderLayer render_layer
)
{
    drawRect(RectShape(x, y, w, h), color, hollow, render_layer);
}

void
Renderer::drawRect(
    RectShape rect,
    Vec4 color,
    bool hollow,
    RenderLayer render_layer
)
{
    uint64_t key = shapeKey(color, hollow);

    if (shapes_cache->rects.find(key) == shapes_cache->rects.end()) {
        shapes_cache->rects[key] =
            std::make_unique<Rect>(this, RectShape(), color);
        shapes_cache->rects[key]->setHollow(hollow);
    }

    shapes_cache->rects[key]->setShape(rect);
    shapes_cache->rects[key]->setRender(render_layer);
    shapes_cache->rects[key]->draw();
}

void
Renderer::drawCircle(
    float center_x,
    float center_y,
    float radius,
    Vec4 color,
    bool hollow,
    RenderLayer render_layer
)
{
    drawCircle(
        CircleShape({ center_x, center_y }, radius, 16),
        color,
        hollow,
        render_layer
    );
}

void
Renderer::drawCircle(
    CircleShape circle,
    Vec4 color,
    bool hollow,
    RenderLayer render_layer
)
{
    uint64_t key = shapeKey(color, hollow);

    if (shapes_cache->circles.find(key) == shapes_cache->circles.end()) {
        shapes_cache->circles[key] =
            std::make_unique<Circle>(this, CircleShape(), color);
        shapes_cache->circles[key]->setHollow(hollow);
    }

    shapes_cache->circles[key]->setShape(circle);
    shapes_cache->circles[key]->setRender(render_layer);
    shapes_cache->circles[key]->draw();
}

void
Renderer::drawLine(
    float start_x,
    float start_y,
    float end_x,
    float end_y,
    Vec4 color,
    float thickness,
    RenderLayer render_layer
)
{
    drawLine(
        LineShape({ start_x, start_y }, { end_x, end_y }, thickness),
        color,
        render_layer
    );
}

void
Renderer::drawLine(LineShape line, Vec4 color, RenderLayer render_layer)
{
    uint32_t key = colorToKey(color);

    if (shapes_cache->lines.find(key) == shapes_cache->lines.end()) {
        shapes_cache->lines[key] =
            std::make_unique<Line>(this, LineShape(), color);
    }

    shapes_cache->lines[key]->setShape(line);
    shapes_cache->lines[key]->setRender(render_layer);
    shapes_cache->lines[key]->draw();
}

void
Renderer::setDefaultFont(BitmapFont* font) noexcept
{
    shapes_cache->custom_default_font = font;
}

BitmapFont*
Renderer::getDefaultFont()
{
    if (shapes_cache->custom_default_font) {
        return shapes_cache->custom_default_font;
    }
    if (!shapes_cache->default_font) {
        shapes_cache->default_font = std::make_unique<BitmapFont>(
            this,
            default_font_png,
            default_font_png_len,
            default_font_cols,
            default_font_rows
        );
    }
    return shapes_cache->default_font.get();
}

void
Renderer::drawText(
    const std::string& text,
    Vec2 pos,
    Vec4 color,
    float scale,
    RenderLayer render_layer
)
{
    drawText(text, pos, getDefaultFont(), color, scale, render_layer);
}

void
Renderer::drawText(
    const std::string& text,
    Vec2 pos,
    BitmapFont* font,
    Vec4 color,
    float scale,
    RenderLayer render_layer
)
{
    if (text.empty())
        return;
    if (!font) {
        font = getDefaultFont();
    }
    if (!font)
        return;

    uint32_t color_key = colorToKey(color);
    TextKey key{ text, color_key, font };

    shapes_cache->usage_counter++;
    uint64_t current_frame = shapes_cache->current_frame;

    auto it = shapes_cache->text_map.find(key);
    if (it != shapes_cache->text_map.end()) {
        auto& entry = shapes_cache->text_entries[it->second];
        entry.last_frame_used = current_frame;
        entry.last_used_counter = shapes_cache->usage_counter;
        entry.text_obj->setPosition(pos);
        entry.text_obj->setScale(scale);
        entry.text_obj->setRender(render_layer);
        entry.text_obj->draw();
        return;
    }

    if (shapes_cache->text_entries.size() >= ShapesCache::MAX_TEXT_CACHE_SIZE) {
        size_t best_idx = size_t(-1);
        uint64_t oldest_counter = UINT64_MAX;
        for (size_t i = 0; i < shapes_cache->text_entries.size(); ++i) {
            if (shapes_cache->text_entries[i].last_frame_used < current_frame) {
                if (shapes_cache->text_entries[i].last_used_counter <
                    oldest_counter) {
                    oldest_counter =
                        shapes_cache->text_entries[i].last_used_counter;
                    best_idx = i;
                }
            }
        }

        if (best_idx != size_t(-1)) {
            auto& entry = shapes_cache->text_entries[best_idx];
            shapes_cache->text_map.erase(entry.key);

            bool same_font = (entry.key.font == font);
            entry.key = key;
            entry.last_frame_used = current_frame;
            entry.last_used_counter = shapes_cache->usage_counter;

            if (entry.text_obj && same_font) {
                entry.text_obj->setText(text);
                entry.text_obj->setColor(color);
            } else {
                entry.text_obj = std::make_unique<Text>(this, font, text);
                entry.text_obj->setColor(color);
            }

            shapes_cache->text_map[key] = best_idx;
            entry.text_obj->setPosition(pos);
            entry.text_obj->setScale(scale);
            entry.text_obj->setRender(render_layer);
            entry.text_obj->draw();
            return;
        }
    }

    auto text_obj = std::make_unique<Text>(this, font, text);
    text_obj->setColor(color);
    size_t idx = shapes_cache->text_entries.size();
    shapes_cache->text_entries.push_back(
        { key, std::move(text_obj), current_frame, shapes_cache->usage_counter }
    );
    shapes_cache->text_map[key] = idx;

    auto& entry = shapes_cache->text_entries[idx];
    entry.text_obj->setPosition(pos);
    entry.text_obj->setScale(scale);
    entry.text_obj->setRender(render_layer);
    entry.text_obj->draw();
}

void
Renderer::drawTextId(
    std::string_view id,
    const std::string& text,
    Vec2 pos,
    Vec4 color,
    float scale,
    RenderLayer render_layer
)
{
    drawTextId(id, text, pos, getDefaultFont(), color, scale, render_layer);
}

void
Renderer::drawTextId(
    std::string_view id,
    const std::string& text,
    Vec2 pos,
    BitmapFont* font,
    Vec4 color,
    float scale,
    RenderLayer render_layer
)
{
    if (text.empty() || id.empty())
        return;
    if (!font)
        font = getDefaultFont();
    if (!font)
        return;

    std::string id_str(id);
    auto it = shapes_cache->id_texts.find(id_str);
    if (it == shapes_cache->id_texts.end()) {
        auto text_obj = std::make_unique<Text>(this, font, text);
        text_obj->setColor(color);
        text_obj->setPosition(pos);
        text_obj->setScale(scale);
        text_obj->setRender(render_layer);
        text_obj->draw();
        shapes_cache->id_texts.emplace(std::move(id_str), std::move(text_obj));
    } else {
        auto& text_obj = it->second;
        text_obj->setText(text);
        text_obj->setColor(color);
        text_obj->setPosition(pos);
        text_obj->setScale(scale);
        text_obj->setRender(render_layer);
        text_obj->draw();
    }
}

void
Renderer::initDevice(SDL_GPUPresentMode preferred_mode)
{
    if (!SDL_ShaderCross_Init())
        throw std::runtime_error(
            "SDL_ShaderCross_Init failed!\n-> " + std::string(SDL_GetError())
        );
    SDL_GPUShaderFormat formats = SDL_ShaderCross_GetSPIRVShaderFormats();
    device = std::unique_ptr<SDL_GPUDevice, SDLGPUDeviceDeleter>(
        SDL_CreateGPUDevice(formats, true, nullptr)
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
Renderer::initShaders()
{
    main_shader = Shader::fromSource(
        device.get(), shaders::world_2d_vert_hlsl, shaders::world_2d_frag_hlsl
    );
}

void
Renderer::initPipelines()
{
    main_pipeline = std::make_unique<MainGraphicsPipeline>(
        device.get(), window->getSdlWindow(), main_shader.get()
    );
}

void
Renderer::initPasses()
{
    render_pass =
        std::make_unique<MainRenderPass>(main_pipeline->getSdlPipeline());
}

void
Renderer::offscreenRender()
{
    Vec2 logical_res = getLogicalResolution();
    uint32_t target_w = static_cast<uint32_t>(logical_res.x);
    uint32_t target_h = static_cast<uint32_t>(logical_res.y);

    bool not_same_size =
        target_w != offscreen_width || target_h != offscreen_height;
    if (!current_offscreen_texture || not_same_size) {
        if (current_offscreen_texture)
            current_offscreen_texture.reset(nullptr);

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
    color_ti.clear_color = SDL_FColor(0.0f, 0.0f, 0.0f, 1.0f);
    color_ti.load_op = SDL_GPU_LOADOP_CLEAR;
    color_ti.store_op = SDL_GPU_STOREOP_STORE;

    SDL_GPURenderPass* pass =
        SDL_BeginGPURenderPass(current_cmd_buffer, &color_ti, 1, nullptr);

    SDL_GPUViewport vp(
        0.0f,
        0.0f,
        static_cast<float>(target_w),
        static_cast<float>(target_h),
        0.0f,
        1.0f
    );
    SDL_SetGPUViewport(pass, &vp);

    render_pass->render(
        pass, current_cmd_buffer, proj_view_world2d, pixel_world_2d_queue
    );

    SDL_EndGPURenderPass(pass);
}

void
Renderer::swapchainRender()
{
    SDL_GPUColorTargetInfo color_ti{};
    color_ti.texture = current_swapchain_texture;
    color_ti.load_op = SDL_GPU_LOADOP_LOAD;
    color_ti.store_op = SDL_GPU_STOREOP_STORE;

    SDL_GPURenderPass* pass =
        SDL_BeginGPURenderPass(current_cmd_buffer, &color_ti, 1, nullptr);

    SDL_GPUViewport vp(
        viewport_x, viewport_y, viewport_w, viewport_h, 0.0f, 1.0f
    );
    SDL_SetGPUViewport(pass, &vp);

    render_pass->render(
        pass, current_cmd_buffer, proj_view_world2d, world_2d_queue
    );
    render_pass->render(pass, current_cmd_buffer, proj_view_ui, ui_queue);

    SDL_EndGPURenderPass(pass);
}

} // namespace lili
