#include <string>
#include <stdexcept>

#include "lili2d/render/renderer.hpp"
#include "lili2d/render/scene/common/model.hpp"
#include "lili2d/render/scene/common/utils.hpp"
#include "lili2d/render/scene/shapes/rect.hpp"

#include "lili2d/render/white_1x1_png.hpp"
#include "shader/world_2d_vert_spv.hpp"
#include "shader/world_2d_frag_spv.hpp"
// #include "shader/ui_vert_spv.hpp"
// #include "shader/ui_frag_spv.hpp"

#include "lili2d/geometry/mat3x3.hpp"

namespace lili {

Renderer::Renderer(Window *window) : window(window) {
	initDevice();
	initShaders();
	initPipelines();
	initPasses();
	the_white_pixel = std::make_unique<Texture>(
		device.get(), white_1x1_png, white_1x1_png_len
	);
}

Renderer::~Renderer() = default;

SDL_GPUDevice *Renderer::getDevice() const {
	return device.get();
}

bool Renderer::beginFrame() {
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

	Mat3 projection = Mat3::orthographic(
		0.0f, static_cast<float>(width),
		0.0f, static_cast<float>(height)
	);
	Mat3 view = Mat3::identity();
	if (camera) {
		projection = camera->getProjection(
			static_cast<float>(width),
			static_cast<float>(height)
		);
		view = camera->getViewMatrix(
			static_cast<float>(width),
			static_cast<float>(height)
		);
	}
	proj_view_world2d = projection * view;

	Mat3 ui_translation = Mat3::translate({ 0.0f, 0.0f });
	Mat3 ui_rotation = Mat3::rotation(0.0f);
	Mat3 ui_scale = Mat3::scale({ 1.0f, 1.0f });
	Mat3 ui_view = ui_scale * ui_rotation * ui_translation;
	proj_view_ui = projection * ui_view;
	return true;
}

void Renderer::submit(
	Model model, const Mat3 &transform, float layer,
	RenderLayer layer_type
) {
	if (layer_type == RenderLayer::WORLD2D)
		world_2d_queue[layer].push_back({ model, transform, layer });
	if (layer_type == RenderLayer::UI)
		ui_queue[layer].push_back({ model, transform, layer });
}

void Renderer::endFrame() {
	SDL_GPUColorTargetInfo color_ti{};
	color_ti.texture = current_swapchain_texture;
	color_ti.clear_color = SDL_FColor{ 0.1f, 0.1f, 0.1f, 1.0f };
	color_ti.load_op = SDL_GPU_LOADOP_CLEAR;
	color_ti.store_op = SDL_GPU_STOREOP_STORE;

	SDL_GPURenderPass *main_pass = SDL_BeginGPURenderPass(
		current_cmd_buffer,	&color_ti, 1, nullptr
	);

	world_2d_pass->render(
		main_pass, current_cmd_buffer, proj_view_world2d, world_2d_queue
	);

	ui_pass->render(
		main_pass, current_cmd_buffer, proj_view_ui, ui_queue
	);

	world_2d_queue.clear();
	ui_queue.clear();
	SDL_EndGPURenderPass(main_pass);
	SDL_SubmitGPUCommandBuffer(current_cmd_buffer);
	current_cmd_buffer = nullptr;
}

Texture *Renderer::getTheWhitePixel() const {
	return the_white_pixel.get();
}

GPUMesh* Renderer::getUnitQuad() {
	if (!unit_quad) {
		unit_quad = std::make_unique<GPUMesh>(device.get(), createUnitQuad());
	}
	return unit_quad.get();
}

GPUMesh* Renderer::getUnitCircle(int segments) {
	if (unit_circles.find(segments) == unit_circles.end()) {
		unit_circles[segments] = std::make_unique<GPUMesh>(
			device.get(), createUnitCircle(segments)
		);
	}
	return unit_circles[segments].get();
}

void Renderer::drawDebugRect(
	float x, float y, float w, float h, const Vec4 &color
) {
	uint32_t r = (uint32_t)(color.x * 255.0f);
	uint32_t g = (uint32_t)(color.y * 255.0f);
	uint32_t b = (uint32_t)(color.z * 255.0f);
	uint32_t a = (uint32_t)(color.w * 255.0f);
	uint32_t key = (r << 24) | (g << 16) | (b << 8) | a;

	if (debug_rects.find(key) == debug_rects.end()) {
		debug_rects[key] = std::make_unique<Rect>(
			this, RectShape(0, 0, 0, 0), color
		);
		debug_rects[key]->setHollow(true);
	}

	debug_rects[key]->setShape(RectShape(x, y, w, h));
	debug_rects[key]->draw();
}

void Renderer::initDevice() {
	device = std::unique_ptr<SDL_GPUDevice, SDLGPUDeviceDeleter>(
		SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr)
	);
	if (!device) {
		throw std::runtime_error(
			"Device creation failed!\n-> " + std::string(SDL_GetError())
		);
	}
	if (!SDL_ClaimWindowForGPUDevice(device.get(), window->getSdlWindow())) {
		throw std::runtime_error(
			"SDL_ClaimWindowForGPUDevice() failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	if (!SDL_SetGPUSwapchainParameters(
		device.get(),
		window->getSdlWindow(),
		SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
		SDL_GPU_PRESENTMODE_MAILBOX
	))
		if (!SDL_SetGPUSwapchainParameters(
			device.get(),
			window->getSdlWindow(),
			SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
			SDL_GPU_PRESENTMODE_IMMEDIATE
		))
			SDL_SetGPUSwapchainParameters(
				device.get(),
				window->getSdlWindow(),
				SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
				SDL_GPU_PRESENTMODE_VSYNC
			);
}

void Renderer::initShaders() {
	world_2d_shader = std::make_unique<Shader>(
		device.get(),
		world_2d_vert_spv, world_2d_vert_spv_len,
		world_2d_frag_spv, world_2d_frag_spv_len,
		ShaderInfo({ .num_uniform_buffers = 1 }),
		ShaderInfo({ .num_samplers = 1, })
	);
}

void Renderer::initPipelines() {
	main_pipeline = std::make_unique<MainGraphicsPipeline>(
		device.get(), window->getSdlWindow(), world_2d_shader.get()
	);
}

void Renderer::initPasses() {
	world_2d_pass = std::make_unique<MainRenderPass>(
		device.get(), main_pipeline->getSdlPipeline()
	);
	ui_pass = std::make_unique<MainRenderPass>(
		device.get(), main_pipeline->getSdlPipeline()
	);
}

void Renderer::setCamera(Camera *camera) {
	this->camera = camera;
}

Camera *Renderer::getCamera() const {
	return camera;
}

uint32_t Renderer::getSwapchainWidth() const {
	return swapchain_width;
}

uint32_t Renderer::getSwapchainHeight() const {
	return swapchain_height;
}

void Renderer::setPresentMode(SDL_GPUPresentMode mode) {
	if (!SDL_SetGPUSwapchainParameters(
		device.get(),
		window->getSdlWindow(),
		SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
		mode
	))
		SDL_SetGPUSwapchainParameters(
			device.get(),
			window->getSdlWindow(),
			SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
			SDL_GPU_PRESENTMODE_VSYNC
		);
}

Shader* Renderer::createShader(
	const std::string &vert_path,
	const std::string &frag_path,
	ShaderInfo vert_infos,
	ShaderInfo frag_infos
) {
	return new Shader(
		device.get(), vert_path, frag_path, vert_infos, frag_infos
	);
}

Shader* Renderer::createShader(
	const uint8_t *vert_code,
	size_t vert_size,
	const uint8_t *frag_code,
	size_t frag_size,
	ShaderInfo vert_infos,
	ShaderInfo frag_infos
) {
	return new Shader(
		device.get(), vert_code, vert_size, frag_code, frag_size,
		vert_infos, frag_infos
	);
}

MainGraphicsPipeline* Renderer::createMainGraphicsPipeline(Shader *shader) {
	return new MainGraphicsPipeline(
		device.get(), window->getSdlWindow(), shader
	);
}

}  // namespace lili
