#pragma once

#include <SDL3/SDL.h>
#include <map>
#include <vector>

#include "core/window.hpp"

#include "render/core/texture.hpp"
#include "render/core/shader.hpp"

#include "render/scene/camera.hpp"
#include "render/scene/model.hpp"

#include "render/passes/pass_types.hpp"
#include "render/passes/world_2d_pass.hpp"
#include "render/passes/ui_pass.hpp"

#include "render/pipelines/world_2d_pipeline.hpp"
#include "render/pipelines/ui_pipeline.hpp"

namespace lili {

/**
 * \brief Main renderer class responsible for handling drawing operations.
 */
class Renderer {
public:
	/**
	 * \brief Constructor for the renderer.
	 * \param window The window to render to.
	 */
	Renderer(Window *window);
	/// \brief Destructor.
	~Renderer();

	/// \brief Copy constructor is deleted to prevent double-freeing the SDL GPU device.
	Renderer(const Renderer &) = delete;
	/// \brief Copy assignment is deleted to prevent double-freeing the SDL GPU device.
	Renderer& operator=(const Renderer &) = delete;

	/**
	 * \brief Gets the SDL GPU device.
	 * \return Pointer to the SDL_GPUDevice.
	 */
	SDL_GPUDevice *get_device() const;

	/**
	 * \brief Begins the rendering frame.
	 * \return True if the frame was successfully started, false otherwise.
	 */
	bool begin_frame();
	/**
	 * \brief Submits a model for rendering.
	 * \param model The model to render.
	 * \param transform The transformation matrix.
	 * \param layer The rendering layer depth.
	 * \param layer_type The type of the rendering layer.
	 */
	void submit(
		Model model, const Mat3 &transform, float layer, RenderLayer layer_type
	);
	/// \brief Ends the rendering frame.
	void end_frame();
	/**
	 * \brief Sets the active camera.
	 * \param camera Pointer to the camera.
	 */
	void set_camera(Camera *camera);

	/**
	 * \brief Gets the default white pixel texture.
	 * \return Pointer to the white pixel texture.
	 */
	Texture *get_the_white_pixel() const;

	/**
	 * \brief Gets the shared unit quad mesh.
	 * \return Pointer to the shared GPUMesh for a unit quad.
	 */
	GPUMesh* get_unit_quad();

	/**
	 * \brief Gets or creates a shared unit circle mesh.
	 * \param segments The number of segments (resolution) of the circle.
	 * \return Pointer to the shared GPUMesh for a unit circle.
	 */
	GPUMesh* get_unit_circle(int segments);

private:
	Window *window = nullptr;
	SDL_GPUDevice *device = nullptr;

	uint32_t swapchain_width, swapchain_height = 0;
	SDL_GPUTexture *current_swapchain_texture = nullptr;
	SDL_GPUCommandBuffer *current_cmd_buffer = nullptr;

	Shader *world_2d_shader = nullptr;
	WorldPipeline *world_2d_pipeline = nullptr;
	World2DPass *world_2d_pass = nullptr;
	std::map<float, std::vector<DrawCommand>> world_2d_queue;

	UIPipeline *ui_pipeline = nullptr;
	UIPass *ui_pass = nullptr;
	std::map<float, std::vector<DrawCommand>> ui_queue;

	Mat3 proj_view_world2d;
	Mat3 proj_view_ui;
	Camera *camera = nullptr;

	Texture *the_white_pixel = nullptr;
	GPUMesh *unit_quad = nullptr;
	std::map<int, GPUMesh*> unit_circles;

	/// \brief init_device method.
	void init_device();
	/// \brief init_shaders method.
	void init_shaders();
	/// \brief init_pipelines method.
	void init_pipelines();
	/// \brief init_passes method.
	void init_passes();
};

}  // namespace lili
