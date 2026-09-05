#pragma once

#include <SDL3/SDL_gpu.h>

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "lili2d/core/sdl_deleters.hpp"
#include "lili2d/core/window.hpp"
#include "lili2d/geometry/shapes2d.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/core/gpu_mesh.hpp"
#include "lili2d/render/core/shader.hpp"
#include "lili2d/render/core/texture.hpp"
#include "lili2d/render/passes/main_render_pass.hpp"
#include "lili2d/render/passes/pass_types.hpp"
#include "lili2d/render/pipelines/main_graphics_pipeline.hpp"
#include "lili2d/world/camera.hpp"

namespace lili {

/// @brief PIMPL cache for primitive shape rendering (Rect, Circle, Line).
struct ShapesCache;

/// @brief Main renderer class responsible for handling drawing operations.
class Renderer {
public:
    /// @brief Constructor for the renderer.
    /// @param window The window to render to.
    /// @param preferred_mode The preferred present mode for the swapchain.
    explicit Renderer(
        Window* window,
        SDL_GPUPresentMode preferred_mode = SDL_GPU_PRESENTMODE_MAILBOX
    );
    /// @brief Destructor.
    ~Renderer();
    /// @brief Move constructor.
    Renderer(Renderer&& other) noexcept;
    /// @brief Move assignment operator.
    /// @param other Renderer instance to move from.
    /// @return Reference to this Renderer.
    Renderer&
    operator=(Renderer&& other) noexcept;
    /// @brief Copy constructor is deleted to prevent double-freeing the SDL
    /// GPU device.
    Renderer(const Renderer&) = delete;
    /// @brief Copy assignment is deleted to prevent double-freeing the SDL GPU
    /// device.
    Renderer&
    operator=(const Renderer&) = delete;

    /// @brief Gets the SDL GPU device.
    /// @return Pointer to the SDL_GPUDevice.
    [[nodiscard]] inline SDL_GPUDevice*
    getDevice() const noexcept {
        return device.get();
    }

    /// @brief Begins the rendering frame.
    /// @return True if the frame was successfully started, false otherwise.
    bool
    beginFrame();
    /// @brief Submits a model for rendering.
    /// @param model The model to render.
    /// @param transform The transformation matrix.
    /// @param layer The rendering layer depth.
    /// @param layer_type The type of the rendering layer.
    void
    submit(
        Model model, const Mat3& transform, float layer, RenderLayer layer_type
    );
    /// @brief Ends the rendering frame.
    void
    endFrame();

    /// @brief Sets the active camera.
    /// @param camera Pointer to the camera.
    inline void
    setCamera(Camera* camera) noexcept {
        this->camera = camera;
    }
    /// @brief Gets the active camera.
    /// @return Pointer to the active camera.
    [[nodiscard]] inline Camera*
    getCamera() const noexcept {
        return camera;
    }

    /// @brief Gets the swapchain width.
    /// @return The width in pixels.
    [[nodiscard]] inline uint32_t
    getSwapchainWidth() const noexcept {
        return swapchain_width;
    }
    /// @brief Gets the swapchain height.
    /// @return The height in pixels.
    [[nodiscard]] inline uint32_t
    getSwapchainHeight() const noexcept {
        return swapchain_height;
    }

    /// @brief Sets the active present mode.
    /// @param mode New present mode
    void
    setPresentMode(SDL_GPUPresentMode mode);

    /// @brief Sets an explicit logical (virtual) resolution on the renderer.
    /// @param width Logical width in pixels (0 to use window logical/physical
    /// size).
    /// @param height Logical height in pixels (0 to use window logical/physical
    /// size).
    inline void
    setLogicalResolution(int width, int height) noexcept {
        logical_width = width;
        logical_height = height;
    }
    /// @brief Gets the active logical resolution used for projections.
    /// @return Vec2 containing logical width and height.
    [[nodiscard]] Vec2
    getLogicalResolution() const;

    /// @brief Creates a shader from HLSL file paths.
    /// @param vert_path Path to the vertex shader file (.hlsl).
    /// @param frag_path Path to the fragment shader file (.hlsl).
    /// @param vert_entry Entry point for vertex shader.
    /// @param frag_entry Entry point for fragment shader.
    /// @return A new Shader instance.
    Shader*
    createShader(
        const std::string& vert_path, const std::string& frag_path,
        const std::string& vert_entry = "main",
        const std::string& frag_entry = "main"
    );
    /// @brief Creates a shader from HLSL source strings.
    /// @param vert_source Vertex HLSL source code.
    /// @param frag_source Fragment HLSL source code.
    /// @param vert_entry Entry point for vertex shader.
    /// @param frag_entry Entry point for fragment shader.
    /// @return A new Shader instance.
    Shader*
    createShader(
        std::string_view vert_source, std::string_view frag_source,
        const std::string& vert_entry = "main",
        const std::string& frag_entry = "main"
    );

    /// @brief Creates a custom graphics pipeline with a given shader.
    /// @param shader The custom shader.
    /// @return A new MainGraphicsPipeline instance.
    MainGraphicsPipeline*
    createMainGraphicsPipeline(Shader* shader);

    /// @brief Gets the default white pixel texture.
    /// @return Pointer to the white pixel texture.
    [[nodiscard]] inline Texture*
    getTheWhitePixel() const noexcept {
        return the_white_pixel.get();
    }
    /// @brief Gets the shared unit quad mesh.
    /// @return Pointer to the shared GPUMesh for a unit quad.
    GPUMesh*
    getUnitQuad();
    /// @brief Gets or creates a shared unit circle mesh.
    /// @param segments The number of segments (resolution) of the circle.
    /// @return Pointer to the shared GPUMesh for a unit circle.
    GPUMesh*
    getUnitCircle(int segments);

    /// @brief Draws a cached rectangle.
    /// @param x X position.
    /// @param y Y position.
    /// @param w Width.
    /// @param h Height.
    /// @param color The color.
    /// @param hollow Whether the rectangle is hollow (outline only). Default is
    /// false.
    void
    drawRect(
        float x, float y, float w, float h, Vec4 color, bool hollow = false,
        RenderLayer render_layer = RenderLayer::WORLD2D
    );
    /// @brief Draws a cached rectangle from a RectShape.
    /// @param rect The rectangle geometry.
    /// @param color The color.
    /// @param hollow Whether the rectangle is hollow (outline only). Default is
    /// false.
    void
    drawRect(
        RectShape rect, Vec4 color, bool hollow = false,
        RenderLayer render_layer = RenderLayer::WORLD2D
    );
    /// @brief Draws a cached circle.
    /// @param center_x X centered position.
    /// @param center_y Y centered position.
    /// @param radius The radius.
    /// @param color The color.
    /// @param hollow Whether the circle is hollow (outline only). Default is
    /// false.
    void
    drawCircle(
        float center_x, float center_y, float radius, Vec4 color,
        bool hollow = false, RenderLayer render_layer = RenderLayer::WORLD2D
    );
    /// @brief Draws a cached circle from a CircleShape.
    /// @param circle The circle geometry.
    /// @param color The color.
    /// @param hollow Whether the circle is hollow (outline only). Default is
    /// false.
    void
    drawCircle(
        CircleShape circle, Vec4 color, bool hollow = false,
        RenderLayer render_layer = RenderLayer::WORLD2D
    );
    /// @brief Draws a cached line between two points.
    /// @param start_x Start X coordinate.
    /// @param start_y Start Y coordinate.
    /// @param end_x End X coordinate.
    /// @param end_y End Y coordinate.
    /// @param color The color.
    /// @param thickness The thickness of the line. Default is 1.0f.
    void
    drawLine(
        float start_x, float start_y, float end_x, float end_y, Vec4 color,
        float thickness = 1.0f, RenderLayer render_layer = RenderLayer::WORLD2D
    );
    /// @brief Draws a cached line from a LineShape.
    /// @param line The line geometry.
    /// @param color The color.
    void
    drawLine(
        LineShape line, Vec4 color,
        RenderLayer render_layer = RenderLayer::WORLD2D
    );

    /// @brief Draws a cached hollow debug rectangle.
    /// @param x X position.
    /// @param y Y position.
    /// @param w Width.
    /// @param h Height.
    /// @param color The color.
    inline void
    drawDebugRect(float x, float y, float w, float h, Vec4 color) {
        drawRect(x, y, w, h, color, true);
    }
    /// @brief Draws a cached hollow debug rectangle from a RectShape.
    /// @param rect The rectangle geometry.
    /// @param color The color.
    inline void
    drawDebugRect(RectShape rect, Vec4 color) {
        drawRect(rect, color, true);
    }
    /// @brief Draws a cached hollow debug circle.
    /// @param center_x X centered position.
    /// @param center_y Y centered position.
    /// @param radius The radius.
    /// @param color The color.
    inline void
    drawDebugCircle(float center_x, float center_y, float radius, Vec4 color) {
        drawCircle(center_x, center_y, radius, color, true);
    }
    /// @brief Draws a cached hollow debug circle from a CircleShape.
    /// @param circle The circle geometry.
    /// @param color The color.
    inline void
    drawDebugCircle(CircleShape circle, Vec4 color) {
        drawCircle(circle, color, true);
    }

private:
    Window* window = nullptr;
    std::unique_ptr<SDL_GPUDevice, SDLGPUDeviceDeleter> device;

    uint32_t swapchain_width = 0;
    uint32_t swapchain_height = 0;

    int logical_width = 0;
    int logical_height = 0;

    uint32_t offscreen_width = 0;
    uint32_t offscreen_height = 0;
    std::unique_ptr<SDL_GPUTexture, SDLGPUTextureDeleter>
        current_offscreen_texture = nullptr;

    float viewport_x = 0.0f;
    float viewport_y = 0.0f;
    float viewport_w = 0.0f;
    float viewport_h = 0.0f;

    SDL_GPUTexture* current_swapchain_texture = nullptr;
    SDL_GPUCommandBuffer* current_cmd_buffer = nullptr;

    std::unique_ptr<Shader> main_shader;
    std::unique_ptr<MainGraphicsPipeline> main_pipeline;

    std::unique_ptr<MainRenderPass> render_pass;

    std::map<float, std::vector<DrawCommand>> world_2d_queue{};
    std::map<float, std::vector<DrawCommand>> ui_queue{};
    std::map<float, std::vector<DrawCommand>> pixel_world_2d_queue{};

    Mat3 proj_view_world2d;
    Mat3 proj_view_ui;

    Camera* camera = nullptr;

    std::unique_ptr<Texture> the_white_pixel;
    std::unique_ptr<GPUMesh> unit_quad;
    std::map<int, std::unique_ptr<GPUMesh>> unit_circles;

    std::unique_ptr<ShapesCache> shapes_cache;

    void
    initDevice(SDL_GPUPresentMode preferred_mode);
    void
    initShaders();
    void
    initPipelines();
    void
    initPasses();

    void
    offscreenRender();
    void
    swapchainRender();
};

}  // namespace lili
