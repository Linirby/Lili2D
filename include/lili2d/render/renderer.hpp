#pragma once

#include <SDL3/SDL.h>
#include <map>
#include <vector>
#include <memory>

#include "lili2d/core/sdl_deleters.hpp"

#include "lili2d/core/window.hpp"

#include "lili2d/render/core/texture.hpp"
#include "lili2d/render/core/shader.hpp"

#include "lili2d/render/scene/common/model.hpp"

#include "lili2d/render/passes/pass_types.hpp"
#include "lili2d/render/passes/main_render_pass.hpp"
#include "lili2d/render/pipelines/main_graphics_pipeline.hpp"

#include "lili2d/world/camera.hpp"

namespace lili {

/// @brief A renderable rectangle object.
class Rect;

/// @brief Main renderer class responsible for handling drawing operations.
class Renderer {
public:
	/// @brief Constructor for the renderer.
	/// @param window The window to render to.
	/// @param preferred_mode The preferred present mode for the swapchain.
	Renderer(
	    Window *window,
		SDL_GPUPresentMode preferred_mode = SDL_GPU_PRESENTMODE_MAILBOX
	);
	/// @brief Destructor.
	~Renderer();
	/// @brief Move constructor.
	Renderer(Renderer &&other) noexcept = default;
	/// @brief Move assignment.
	Renderer& operator=(Renderer &&other) noexcept = default;
	/// @brief Copy constructor is deleted to prevent double-freeing the SDL GPU
	/// device.
	Renderer(const Renderer &) = delete;
	/// @brief Copy assignment is deleted to prevent double-freeing the SDL GPU
	/// device.
	Renderer& operator=(const Renderer &) = delete;

	/// @brief Gets the SDL GPU device.
	/// @return Pointer to the SDL_GPUDevice.
	SDL_GPUDevice *getDevice() const;

	/// @brief Begins the rendering frame.
	/// @return True if the frame was successfully started, false otherwise.
	bool beginFrame();
	/// @brief Submits a model for rendering.
	/// @param model The model to render.
	/// @param transform The transformation matrix.
	/// @param layer The rendering layer depth.
	/// @param layer_type The type of the rendering layer.
	void submit(
		Model model, const Mat3 &transform, float layer, RenderLayer layer_type
	);
	/// @brief Ends the rendering frame.
	void endFrame();
	/// @brief Sets the active camera.
	/// @param camera Pointer to the camera.
	void setCamera(Camera *camera);
	/// @brief Gets the active camera.
	/// @return Pointer to the active camera.
	Camera *getCamera() const;
	/// @brief Gets the swapchain width.
	/// @return The width in pixels.
	uint32_t getSwapchainWidth() const;
	/// @brief Gets the swapchain height.
	/// @return The height in pixels.
	uint32_t getSwapchainHeight() const;
	/// @brief Sets the active present mode.
	/// @param mode New present mode
	void setPresentMode(SDL_GPUPresentMode mode);

	/// @brief Creates a shader from file paths.
	/// @param vert_path Path to the vertex shader file.
	/// @param frag_path Path to the fragment shader file.
	/// @param vert_infos Binding info for the vertex shader.
	/// @param frag_infos Binding info for the fragment shader.
	/// @return A new Shader instance.
	Shader* createShader(
		const std::string &vert_path,
		const std::string &frag_path,
		ShaderInfo vert_infos = {},
		ShaderInfo frag_infos = {}
	);
	/// @brief Creates a shader from memory.
	/// @param vert_code Pointer to the vertex shader code.
	/// @param vert_size Size of the vertex shader code.
	/// @param frag_code Pointer to the fragment shader code.
	/// @param frag_size Size of the fragment shader code.
	/// @param vert_infos Binding info for the vertex shader.
	/// @param frag_infos Binding info for the fragment shader.
	/// @return A new Shader instance.
	Shader* createShader(
		const uint8_t *vert_code,
		size_t vert_size,
		const uint8_t *frag_code,
		size_t frag_size,
		ShaderInfo vert_infos = {},
		ShaderInfo frag_infos = {}
	);

	/// @brief Creates a custom graphics pipeline with a given shader.
	/// @param shader The custom shader.
	/// @return A new MainGraphicsPipeline instance.
	MainGraphicsPipeline* createMainGraphicsPipeline(Shader *shader);

	/// @brief Gets the default white pixel texture.
	/// @return Pointer to the white pixel texture.
	Texture *getTheWhitePixel() const;
	/// @brief Gets the shared unit quad mesh.
	/// @return Pointer to the shared GPUMesh for a unit quad.
	GPUMesh* getUnitQuad();
	/// @brief Gets or creates a shared unit circle mesh.
	/// @param segments The number of segments (resolution) of the circle.
	/// @return Pointer to the shared GPUMesh for a unit circle.
	GPUMesh* getUnitCircle(int segments);

	/// @brief Draws a cached hollow debug rectangle.
	/// @param x X position.
	/// @param y Y position.
	/// @param w Width.
	/// @param h Height.
	/// @param color The color.
	void drawDebugRect(float x, float y, float w, float h, const Vec4 &color);

private:
	Window *window = nullptr;
	std::unique_ptr<SDL_GPUDevice, SDLGPUDeviceDeleter> device;

	uint32_t swapchain_width, swapchain_height = 0;
	SDL_GPUTexture *current_swapchain_texture = nullptr;
	SDL_GPUCommandBuffer *current_cmd_buffer = nullptr;

	std::unique_ptr<Shader> world_2d_shader;
	std::unique_ptr<MainGraphicsPipeline> main_pipeline;
	std::unique_ptr<MainRenderPass> world_2d_pass;
	std::map<float, std::vector<DrawCommand>> world_2d_queue;

	std::unique_ptr<MainRenderPass> ui_pass;
	std::map<float, std::vector<DrawCommand>> ui_queue;

	Mat3 proj_view_world2d;
	Mat3 proj_view_ui;
	Camera *camera = nullptr;

	std::unique_ptr<Texture> the_white_pixel;
	std::unique_ptr<GPUMesh> unit_quad;
	std::map<int, std::unique_ptr<GPUMesh>> unit_circles;

	std::map<uint32_t, std::unique_ptr<Rect>> debug_rects;

	/// @brief initDevice method.
	/// @param preferred_mode The preferred present mode for the swapchain.
	void initDevice(SDL_GPUPresentMode preferred_mode);
	/// @brief initShaders method.
	void initShaders();
	/// @brief initPipelines method.
	void initPipelines();
	/// @brief initPasses method.
	void initPasses();
};

}  // namespace lili
