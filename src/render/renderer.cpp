#include <string>
#include <stdexcept>

#include "render/renderer.hpp"
#include "render/scene/model.hpp"

#include "geometry/mat4x4.hpp"

namespace lili {

Renderer::Renderer() {
	device = nullptr;
	depth_texture = nullptr;
	current_swapchain_texture = nullptr;
	current_cmd_buffer = nullptr;
	ui_shader = nullptr;
	ui_pipeline = nullptr;
	ui_pass = nullptr;
}

Renderer::~Renderer() {
	SDL_WaitForGPUIdle(device);

	if (ui_pass) delete ui_pass;
	if (ui_pipeline) delete ui_pipeline;
	if (ui_shader) delete ui_shader;
	if (depth_texture) SDL_ReleaseGPUTexture(device, depth_texture);
	if (device) SDL_DestroyGPUDevice(device);
}

void Renderer::set_window(Window *window) {
	this->window = window;
	init_device();
	init_depth_texture();
	init_shaders();
	init_pipelines();
	init_passes();
}

void Renderer::on_window_resized() {
	SDL_ReleaseGPUTexture(device, depth_texture);
	init_depth_texture();
}

SDL_GPUDevice *Renderer::get_device() const {
	return device;
}

bool Renderer::begin_frame() {
	current_cmd_buffer = SDL_AcquireGPUCommandBuffer(device);
	if (!current_cmd_buffer)
		throw std::runtime_error("Failed to acquire command buffer!");

	uint32_t width = 0;
	uint32_t height = 0;
	bool success = SDL_WaitAndAcquireGPUSwapchainTexture(
		current_cmd_buffer,
		window->get_sdl_window(),
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
		on_window_resized();
	}

	projection_2d = Mat4::orthographic(
		0.0f, static_cast<float>(width),
		static_cast<float>(height), 0.0f,
		-1.0f, 1.0f
	);
	return true;
}

void Renderer::submit(const Model &model, Mat4 transform, RenderLayer layer) {
	if (layer == RenderLayer::UI2D)
		ui_queue.push_back({ model, transform });
}

void Renderer::end_frame() {
	SDL_GPUColorTargetInfo color_target_info{};
	color_target_info.texture = current_swapchain_texture;
	color_target_info.clear_color = SDL_FColor{ 0.1f, 0.1f, 0.1f, 1.0f };
	color_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
	color_target_info.store_op = SDL_GPU_STOREOP_STORE;

	SDL_GPUDepthStencilTargetInfo depth_target_info{};
	depth_target_info.texture = depth_texture;
	depth_target_info.clear_depth = 1.0f;
	depth_target_info.load_op = SDL_GPU_LOADOP_CLEAR;
	depth_target_info.store_op = SDL_GPU_STOREOP_DONT_CARE;
	depth_target_info.stencil_load_op = SDL_GPU_LOADOP_DONT_CARE;
	depth_target_info.stencil_store_op = SDL_GPU_STOREOP_DONT_CARE;
	depth_target_info.cycle = false;
	depth_target_info.clear_stencil = 0;

	SDL_GPURenderPass *main_pass = SDL_BeginGPURenderPass(
		current_cmd_buffer,	&color_target_info, 1, &depth_target_info
	);

	ui_pass->render(
		main_pass, current_cmd_buffer, projection_2d, ui_queue
	);

	ui_queue.clear();
	SDL_EndGPURenderPass(main_pass);
	SDL_SubmitGPUCommandBuffer(current_cmd_buffer);
	current_cmd_buffer = nullptr;
}

void Renderer::init_device() {
	device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr);
	if (!device) {
		throw std::runtime_error(
			"Device creation failed!\n-> " + std::string(SDL_GetError())
		);
	}
	if (!SDL_ClaimWindowForGPUDevice(device, window->get_sdl_window())) {
		throw std::runtime_error(
			"SDL_ClaimWindowForGPUDevice() failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	if (!SDL_SetGPUSwapchainParameters(
		device,
		window->get_sdl_window(),
		SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
		SDL_GPU_PRESENTMODE_MAILBOX
	)) {
		SDL_SetGPUSwapchainParameters(
			device,
			window->get_sdl_window(),
			SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
			SDL_GPU_PRESENTMODE_IMMEDIATE
		);
	}
}

void Renderer::init_depth_texture() {
	int win_w, win_h;
	SDL_GetWindowSize(window->get_sdl_window(), &win_w, &win_h);

	SDL_GPUTextureCreateInfo depth_info{};
	depth_info.type = SDL_GPU_TEXTURETYPE_2D;
	depth_info.format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
	depth_info.usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET;
	depth_info.width = static_cast<uint32_t>(win_w);
	depth_info.height = static_cast<uint32_t>(win_h);
	depth_info.layer_count_or_depth = 1;
	depth_info.num_levels = 1;
	depth_info.sample_count = SDL_GPU_SAMPLECOUNT_1;

	depth_texture = SDL_CreateGPUTexture(device, &depth_info);
	if (!depth_texture)
		throw std::runtime_error(
			"Depth texture creation failed!\n-> " + std::string(SDL_GetError())
		);
}

void Renderer::init_shaders() {
	ui_shader = new Shader(
		device, "shader/ui.vert.spv", "shader/ui.frag.spv",
		(ShaderInfo){
			.num_uniform_buffers = 1
		},
		(ShaderInfo){
			.num_samplers = 1,
			.num_storage_buffers = 1
		}
	);
}

void Renderer::init_pipelines() {
	ui_pipeline = new UIPipeline(device, window->get_sdl_window(), ui_shader);
}

void Renderer::init_passes() {
	ui_pass = new UIPass(device, ui_pipeline->get_sdl_pipeline());
}

}  // namespace lili
