#include <string>
#include <stdexcept>

#include "lili2d/render/renderer.hpp"
#include "lili2d/render/scene/common/model.hpp"
#include "lili2d/render/scene/common/utils.hpp"

// Creation of those 3 headers in cmake
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
	the_white_pixel = new Texture(device, white_1x1_png, white_1x1_png_len);
}

Renderer::~Renderer() {
	if (device) SDL_WaitForGPUIdle(device);

	if (unit_quad) delete unit_quad;
	for (auto& pair : unit_circles)
		delete pair.second;
	if (the_white_pixel) delete the_white_pixel;
	if (ui_pass) delete ui_pass;
	if (ui_pipeline) delete ui_pipeline;
	if (world_2d_pass) delete world_2d_pass;
	if (world_2d_pipeline) delete world_2d_pipeline;
	if (world_2d_shader) delete world_2d_shader;
	if (device) SDL_DestroyGPUDevice(device);
}

Renderer::Renderer(Renderer &&other) noexcept
	: window(other.window),
	device(other.device),
	swapchain_width(other.swapchain_width),
	swapchain_height(other.swapchain_height),
	current_swapchain_texture(other.current_swapchain_texture),
	current_cmd_buffer(other.current_cmd_buffer),
	world_2d_shader(other.world_2d_shader),
	world_2d_pipeline(other.world_2d_pipeline),
	world_2d_pass(other.world_2d_pass),
	world_2d_queue(std::move(other.world_2d_queue)),
	ui_pipeline(other.ui_pipeline),
	ui_pass(other.ui_pass),
	ui_queue(std::move(other.ui_queue)),
	proj_view_world2d(other.proj_view_world2d),
	proj_view_ui(other.proj_view_ui),
	camera(other.camera),
	the_white_pixel(other.the_white_pixel),
	unit_quad(other.unit_quad),
	unit_circles(std::move(other.unit_circles)) {
	other.device = nullptr;
	other.current_cmd_buffer = nullptr;
	other.world_2d_shader = nullptr;
	other.world_2d_pipeline = nullptr;
	other.world_2d_pass = nullptr;
	other.ui_pipeline = nullptr;
	other.ui_pass = nullptr;
	other.the_white_pixel = nullptr;
	other.unit_quad = nullptr;
	other.unit_circles.clear();
}

Renderer& Renderer::operator=(Renderer &&other) noexcept {
	if (this != &other) {
		if (device) SDL_WaitForGPUIdle(device);

		if (unit_quad) delete unit_quad;
		for (auto& pair : unit_circles)
			delete pair.second;
		if (the_white_pixel) delete the_white_pixel;
		if (ui_pass) delete ui_pass;
		if (ui_pipeline) delete ui_pipeline;
		if (world_2d_pass) delete world_2d_pass;
		if (world_2d_pipeline) delete world_2d_pipeline;
		if (world_2d_shader) delete world_2d_shader;
		if (device) SDL_DestroyGPUDevice(device);

		window = other.window;
		device = other.device;
		swapchain_width = other.swapchain_width;
		swapchain_height = other.swapchain_height;
		current_swapchain_texture = other.current_swapchain_texture;
		current_cmd_buffer = other.current_cmd_buffer;
		world_2d_shader = other.world_2d_shader;
		world_2d_pipeline = other.world_2d_pipeline;
		world_2d_pass = other.world_2d_pass;
		world_2d_queue = std::move(other.world_2d_queue);
		ui_pipeline = other.ui_pipeline;
		ui_pass = other.ui_pass;
		ui_queue = std::move(other.ui_queue);
		proj_view_world2d = other.proj_view_world2d;
		proj_view_ui = other.proj_view_ui;
		camera = other.camera;
		the_white_pixel = other.the_white_pixel;
		unit_quad = other.unit_quad;
		unit_circles = std::move(other.unit_circles);

		other.device = nullptr;
		other.current_cmd_buffer = nullptr;
		other.world_2d_shader = nullptr;
		other.world_2d_pipeline = nullptr;
		other.world_2d_pass = nullptr;
		other.ui_pipeline = nullptr;
		other.ui_pass = nullptr;
		other.the_white_pixel = nullptr;
		other.unit_quad = nullptr;
		other.unit_circles.clear();
	}
	return *this;
}

SDL_GPUDevice *Renderer::getDevice() const {
	return device;
}

bool Renderer::beginFrame() {
	current_cmd_buffer = SDL_AcquireGPUCommandBuffer(device);
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

	for (auto& pair : world_2d_queue) pair.second.clear();
	for (auto& pair : ui_queue) pair.second.clear();
	SDL_EndGPURenderPass(main_pass);
	SDL_SubmitGPUCommandBuffer(current_cmd_buffer);
	current_cmd_buffer = nullptr;
}

Texture *Renderer::getTheWhitePixel() const {
	return the_white_pixel;
}

GPUMesh* Renderer::getUnitQuad() {
	if (!unit_quad) {
		unit_quad = new GPUMesh(device, createUnitQuad());
	}
	return unit_quad;
}

GPUMesh* Renderer::getUnitCircle(int segments) {
	if (unit_circles.find(segments) == unit_circles.end()) {
		unit_circles[segments] = new GPUMesh(
			device, createUnitCircle(segments)
		);
	}
	return unit_circles[segments];
}

void Renderer::initDevice() {
	device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr);
	if (!device) {
		throw std::runtime_error(
			"Device creation failed!\n-> " + std::string(SDL_GetError())
		);
	}
	if (!SDL_ClaimWindowForGPUDevice(device, window->getSdlWindow())) {
		throw std::runtime_error(
			"SDL_ClaimWindowForGPUDevice() failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	if (!SDL_SetGPUSwapchainParameters(
		device,
		window->getSdlWindow(),
		SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
		SDL_GPU_PRESENTMODE_MAILBOX
	)) {
		SDL_SetGPUSwapchainParameters(
			device,
			window->getSdlWindow(),
			SDL_GPU_SWAPCHAINCOMPOSITION_SDR,
			SDL_GPU_PRESENTMODE_IMMEDIATE
		);
	}
}

void Renderer::initShaders() {
	world_2d_shader = new Shader(
		device,
		world_2d_vert_spv, world_2d_vert_spv_len,
		world_2d_frag_spv, world_2d_frag_spv_len,
		(ShaderInfo){
			.num_uniform_buffers = 1
		},
		(ShaderInfo){
			.num_samplers = 1,
		}
	);
}

void Renderer::initPipelines() {
	world_2d_pipeline = new WorldPipeline(
		device, window->getSdlWindow(), world_2d_shader
	);
	ui_pipeline = new UIPipeline(
		device, window->getSdlWindow(), world_2d_shader
	);
}

void Renderer::initPasses() {
	world_2d_pass = new World2DPass(
		device, world_2d_pipeline->getSdlPipeline()
	);
	ui_pass = new UIPass(
		device, ui_pipeline->getSdlPipeline()
	);
}

void Renderer::setCamera(Camera *camera) {
	this->camera = camera;
}

Shader* Renderer::createShader(
	const std::string &vert_path,
	const std::string &frag_path,
	ShaderInfo vert_infos,
	ShaderInfo frag_infos
) {
	return new Shader(device, vert_path, frag_path, vert_infos, frag_infos);
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
		device, vert_code, vert_size, frag_code, frag_size,
		vert_infos, frag_infos
	);
}

WorldPipeline* Renderer::createWorldPipeline(Shader *shader) {
	return new WorldPipeline(device, window->getSdlWindow(), shader);
}

UIPipeline* Renderer::createUiPipeline(Shader *shader) {
	return new UIPipeline(device, window->getSdlWindow(), shader);
}

}  // namespace lili
