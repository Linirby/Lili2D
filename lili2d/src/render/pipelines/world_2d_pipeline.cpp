#include "world_2d_pipeline.hpp"

#include <vector>
#include <stdexcept>

#include "../core/shader.hpp"

namespace lili {

WorldPipeline::WorldPipeline(
	SDL_GPUDevice *device, SDL_Window *window, Shader *shader
) : device(device), window(window), shader(shader) {
	SDL_GPUVertexBufferDescription vertex_bd{};
	vertex_bd.slot = 0;
	vertex_bd.pitch = sizeof(float) * 6;
	vertex_bd.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
	vertex_bd.instance_step_rate = 0;

	std::vector<SDL_GPUVertexAttribute> vertexAttrs(3);
	vertexAttrs[0].location = 0;
	vertexAttrs[0].buffer_slot = 0;
	vertexAttrs[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
	vertexAttrs[0].offset = 0;

	vertexAttrs[1].location = 2;
	vertexAttrs[1].buffer_slot = 0;
	vertexAttrs[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
	vertexAttrs[1].offset = sizeof(float) * 3;

	vertexAttrs[2].location = 3;
	vertexAttrs[2].buffer_slot = 0;
	vertexAttrs[2].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT;
	vertexAttrs[2].offset = sizeof(float) * 5;

	SDL_GPUColorTargetDescription color_td{};
	color_td.format = SDL_GetGPUSwapchainTextureFormat(device, window);
	color_td.blend_state.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
	color_td.blend_state.dst_color_blendfactor = (
		SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA
	);
	color_td.blend_state.color_blend_op = SDL_GPU_BLENDOP_ADD;
	color_td.blend_state.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
	color_td.blend_state.dst_alpha_blendfactor = (
		SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA
	);
	color_td.blend_state.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
	color_td.blend_state.color_write_mask = SDL_GPU_COLORCOMPONENT_R |
		SDL_GPU_COLORCOMPONENT_G |
		SDL_GPU_COLORCOMPONENT_B |
		SDL_GPU_COLORCOMPONENT_A;
	color_td.blend_state.enable_blend = true;
	color_td.blend_state.enable_color_write_mask = true;

	SDL_GPUGraphicsPipelineCreateInfo ci{};
	ci.vertex_shader = shader->getVertex();
	ci.fragment_shader = shader->getFragment();
	ci.vertex_input_state.vertex_buffer_descriptions = &vertex_bd;
	ci.vertex_input_state.num_vertex_buffers = 1;
	ci.vertex_input_state.vertex_attributes = vertexAttrs.data();
	ci.vertex_input_state.num_vertex_attributes = (
		static_cast<uint32_t>(vertexAttrs.size())
	);
	ci.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
	ci.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
	ci.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;
	ci.rasterizer_state.front_face = SDL_GPU_FRONTFACE_CLOCKWISE;
	ci.multisample_state.sample_count = SDL_GPU_SAMPLECOUNT_1;
	ci.depth_stencil_state.compare_op = SDL_GPU_COMPAREOP_ALWAYS;
	ci.depth_stencil_state.back_stencil_state.fail_op = SDL_GPU_STENCILOP_ZERO;
	ci.depth_stencil_state.front_stencil_state.fail_op = SDL_GPU_STENCILOP_ZERO;
	ci.depth_stencil_state.compare_mask = 0;
	ci.depth_stencil_state.write_mask = 0;
	ci.depth_stencil_state.enable_depth_test = false;
	ci.depth_stencil_state.enable_depth_write = false;
	ci.depth_stencil_state.enable_stencil_test = false;
	ci.target_info.color_target_descriptions = &color_td;
	ci.target_info.num_color_targets = 1;
	ci.target_info.depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT;
	ci.target_info.has_depth_stencil_target = false;

	pipeline = SDL_CreateGPUGraphicsPipeline(device, &ci);
	if (!pipeline) {
		throw std::runtime_error(
			"World graphics pipeline creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
}

WorldPipeline::~WorldPipeline() {
	if (pipeline) SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
}

WorldPipeline::WorldPipeline(WorldPipeline &&other) noexcept
	: device(other.device),
			window(other.window),
			shader(other.shader),
			pipeline(other.pipeline) {
	other.pipeline = nullptr;
}

WorldPipeline& WorldPipeline::operator=(WorldPipeline &&other) noexcept {
	if (this != &other) {
		if (pipeline) SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
		device = other.device;
		window = other.window;
		shader = other.shader;
		pipeline = other.pipeline;
		other.pipeline = nullptr;
	}
	return *this;
}

SDL_GPUGraphicsPipeline *WorldPipeline::getSdlPipeline() {
	return pipeline;
}

}  // namespace lili
