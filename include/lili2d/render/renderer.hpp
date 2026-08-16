#pragma once

#include <SDL3/SDL.h>

#include <cstdint>
#include <map>
#include <memory>
#include <vector>

#include "lili2d/core/sdl_deleters.hpp"
#include "lili2d/core/window.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/core/shader.hpp"
#include "lili2d/render/core/texture.hpp"
#include "lili2d/render/passes/main_render_pass.hpp"
#include "lili2d/render/passes/pass_types.hpp"
#include "lili2d/render/pipelines/main_graphics_pipeline.hpp"
#include "lili2d/render/scene/common/model.hpp"
#include "lili2d/world/camera.hpp"

namespace lili {

/// @brief A renderable rectangle object.
class Rect;
/// @brief A renderable circle object.
class Circle;

/// @brief Main renderer class responsible for handling drawing operations.
class Renderer {
public:
    /// @brief Constructor for the renderer.
    /// @param window The window to render to.
    /// @param preferred_mode The preferred present mode for the swapchain.
    Renderer(
        Window* window,
        SDL_GPUPresentMode preferred_mode = SDL_GPU_PRESENTMODE_MAILBOX
    );
    /// @brief Destructor.
    ~Renderer();
    /// @brief Move constructor.
    Renderer(Renderer&& other) noexcept = default;
    /// @brief Move assignment operator.
    /// @param other Renderer instance to move from.
    /// @return Reference to this Renderer.
    Renderer&
    operator=(Renderer&& other) noexcept = default;
    /// @brief Copy constructor is deleted to prevent double-freeing the SDL
    /// GPU device.
    Renderer(const Renderer&) = delete;
    /// @brief Copy assignment is deleted to prevent double-freeing the SDL GPU
    /// device.
    Renderer&
    operator=(const Renderer&) = delete;

    /// @brief Gets the SDL GPU device.
    /// @return Pointer to the SDL_GPUDevice.
    SDL_GPUDevice*
    getDevice() const;

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
    void
    setCamera(Camera* camera);
    /// @brief Gets the active camera.
    /// @return Pointer to the active camera.
    Camera*
    getCamera() const;
    /// @brief Gets the swapchain width.
    /// @return The width in pixels.
    uint32_t
    getSwapchainWidth() const;
    /// @brief Gets the swapchain height.
    /// @return The height in pixels.
    uint32_t
    getSwapchainHeight() const;
    /// @brief Sets the active present mode.
    /// @param mode New present mode
    void
    setPresentMode(SDL_GPUPresentMode mode);

    /// @brief Sets an explicit logical (virtual) resolution on the renderer.
    /// @param width Logical width in pixels (0 to use window logical/physical
    /// size).
    /// @param height Logical height in pixels (0 to use window logical/physical
    /// size).
    void
    setLogicalResolution(int width, int height);
    /// @brief Gets the active logical resolution used for projections.
    /// @return Vec2 containing logical width and height.
    Vec2
    getLogicalResolution() const;

    /// @brief Creates a shader from file paths.
    /// @param vert_path Path to the vertex shader file.
    /// @param frag_path Path to the fragment shader file.
    /// @param vert_infos Binding info for the vertex shader.
    /// @param frag_infos Binding info for the fragment shader.
    /// @return A new Shader instance.
    Shader*
    createShader(
        const std::string& vert_path, const std::string& frag_path,
        ShaderInfo vert_infos = {}, ShaderInfo frag_infos = {}
    );
    /// @brief Creates a shader from memory.
    /// @param vert_code Pointer to the vertex shader code.
    /// @param vert_size Size of the vertex shader code.
    /// @param frag_code Pointer to the fragment shader code.
    /// @param frag_size Size of the fragment shader code.
    /// @param vert_infos Binding info for the vertex shader.
    /// @param frag_infos Binding info for the fragment shader.
    /// @return A new Shader instance.
    Shader*
    createShader(
        const uint8_t* vert_code, size_t vert_size, const uint8_t* frag_code,
        size_t frag_size, ShaderInfo vert_infos = {}, ShaderInfo frag_infos = {}
    );

    /// @brief Creates a custom graphics pipeline with a given shader.
    /// @param shader The custom shader.
    /// @return A new MainGraphicsPipeline instance.
    MainGraphicsPipeline*
    createMainGraphicsPipeline(Shader* shader);

    /// @brief Gets the default white pixel texture.
    /// @return Pointer to the white pixel texture.
    Texture*
    getTheWhitePixel() const;
    /// @brief Gets the shared unit quad mesh.
    /// @return Pointer to the shared GPUMesh for a unit quad.
    GPUMesh*
    getUnitQuad();
    /// @brief Gets or creates a shared unit circle mesh.
    /// @param segments The number of segments (resolution) of the circle.
    /// @return Pointer to the shared GPUMesh for a unit circle.
    GPUMesh*
    getUnitCircle(int segments);

    /// @brief Draws a cached hollow debug rectangle.
    /// @param x X position.
    /// @param y Y position.
    /// @param w Width.
    /// @param h Height.
    /// @param color The color.
    void
    drawDebugRect(float x, float y, float w, float h, const Vec4& color);
    /// @brief Draws a cached hollow debug circle.
    /// @param center_x X centered position.
    /// @param center_y Y centered position.
    /// @param radius The Radius.
    /// @param color The color.
    void
    drawDebugCircle(
        float center_x, float center_y, float radius, const Vec4& color
    );

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

    std::unique_ptr<Shader> main_shader = nullptr;
    std::unique_ptr<MainGraphicsPipeline> main_pipeline = nullptr;

    std::unique_ptr<MainRenderPass> render_pass = nullptr;

    std::map<float, std::vector<DrawCommand>> world_2d_queue{};
    std::map<float, std::vector<DrawCommand>> ui_queue{};
    std::map<float, std::vector<DrawCommand>> pixel_world_2d_queue{};

    Mat3 proj_view_world2d;
    Mat3 proj_view_ui;

    Camera* camera = nullptr;

    std::unique_ptr<Texture> the_white_pixel;
    std::unique_ptr<GPUMesh> unit_quad;
    std::map<int, std::unique_ptr<GPUMesh>> unit_circles;

    std::map<uint32_t, std::unique_ptr<Rect>> debug_rects;
    std::map<uint32_t, std::unique_ptr<Circle>> debug_circles;

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
