#pragma once

#include <SDL3/SDL.h>

#include "core/window.hpp"

#include "render/core/texture.hpp"
#include "render/core/shader.hpp"

#include "render/scene/camera.hpp"
#include "render/scene/model.hpp"

#include "render/passes/pass_types.hpp"
#include "render/passes/world_2d_pass.hpp"

#include "render/pipelines/world_2d_pipeline.hpp"

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

private:
	Window *window;
	SDL_GPUDevice *device;

	uint32_t swapchain_width, swapchain_height;
	SDL_GPUTexture *current_swapchain_texture;
	SDL_GPUCommandBuffer *current_cmd_buffer;

	Shader *world_2d_shader;
	UIPipeline *world_2d_pipeline;
	World2DPass *world_2d_pass;
	std::vector<DrawCommand> world_2d_queue;

	Mat3 projection_view_2d;
	Camera *camera = nullptr;

	Texture *the_white_pixel;

	void init_device();
	void init_shaders();
	void init_pipelines();
	void init_passes();
};

}  // namespace lili
