#pragma once

#include <SDL3/SDL.h>

#include "core/window.hpp"

#include "render/core/shader.hpp"
#include "render/scene/camera.hpp"
#include "render/scene/model.hpp"
#include "render/passes/pass_types.hpp"
#include "render/passes/ui_pass.hpp"
#include "render/pipelines/ui_pipeline.hpp"

namespace lili {

class Renderer {
public:
	Renderer(Window *window);
	~Renderer();

	SDL_GPUDevice *get_device() const;

	bool begin_frame();
	void submit(
		const Model &model, const Mat3 &transform, float layer, RenderLayer layer_type
	);
	void end_frame();
	void set_camera(Camera *camera);

private:
	Window *window;
	SDL_GPUDevice *device;

	uint32_t swapchain_width, swapchain_height;
	SDL_GPUTexture *current_swapchain_texture;
	SDL_GPUCommandBuffer *current_cmd_buffer;

	Shader *world_2d_shader;

	UIPipeline *world_2d_pipeline;

	UIPass *world_2d_pass;

	std::vector<DrawCommand> world_2d_queue;

	Mat3 projection_view_2d;
	Camera *camera = nullptr;

	void init_device();
	void init_shaders();
	void init_pipelines();
	void init_passes();
};

}  // namespace lili
