#include "render/passes/world_2d_pass.hpp"

#include <stdexcept>
#include <vector>

#include "render/scene/model.hpp"

namespace lili {

World2DPass::World2DPass(
	SDL_GPUDevice *device, SDL_GPUGraphicsPipeline *pipeline
) {
	this->device = device;
	this->pipeline = pipeline;
}

void World2DPass::render(
	SDL_GPURenderPass *pass,
	SDL_GPUCommandBuffer *cmd,
	const Mat3 &proj_view,
	const std::vector<DrawCommand> &queue
) {
	if (queue.empty()) return;

	SDL_BindGPUGraphicsPipeline(pass, pipeline);
	for (const DrawCommand &draw_cmd : queue) {

		if (!draw_cmd.model.mesh)
			throw std::runtime_error(
				"World2DPass received draw command without mesh."
			);
		if (!draw_cmd.model.material)
			throw std::runtime_error(
				"World2DPass received draw command without material."
			);
		if (!draw_cmd.model.material->albedo_map)
			throw std::runtime_error(
				"World2DPass received draw command with material missing "
				"albedo map."
			);

		Mat3 mvp = proj_view * draw_cmd.transform;
		struct UIUniforms {
			float matrix[12];
			float color[4];
			float layer;
			float padding[3];
		};
		UIUniforms uniforms{};
		uniforms.matrix[0] = mvp.m[0];
		uniforms.matrix[1] = mvp.m[1];
		uniforms.matrix[2] = mvp.m[2];
		uniforms.matrix[3] = 0.0f;
		uniforms.matrix[4] = mvp.m[3];
		uniforms.matrix[5] = mvp.m[4];
		uniforms.matrix[6] = mvp.m[5];
		uniforms.matrix[7] = 0.0f;
		uniforms.matrix[8] = mvp.m[6];
		uniforms.matrix[9] = mvp.m[7];
		uniforms.matrix[10] = mvp.m[8];
		uniforms.matrix[11] = 0.0f;
		uniforms.color[0] = draw_cmd.model.material->properties.color_tint.x;
		uniforms.color[1] = draw_cmd.model.material->properties.color_tint.y;
		uniforms.color[2] = draw_cmd.model.material->properties.color_tint.z;
		uniforms.color[3] = draw_cmd.model.material->properties.color_tint.w;
		uniforms.layer = draw_cmd.layer;

		SDL_PushGPUVertexUniformData(cmd, 0, &uniforms, sizeof(uniforms));

		SDL_GPUBufferBinding vertex_binding{
			.buffer = draw_cmd.model.mesh->get_vertex(),
			.offset = 0
		};
		SDL_BindGPUVertexBuffers(pass, 0, &vertex_binding, 1);

		SDL_GPUBufferBinding index_binding{
			.buffer = draw_cmd.model.mesh->get_index(),
			.offset = 0
		};
		SDL_BindGPUIndexBuffer(
			pass, &index_binding, SDL_GPU_INDEXELEMENTSIZE_32BIT
		);

		SDL_GPUTextureSamplerBinding texture_sampler_binding{
			.texture = draw_cmd.model.material->albedo_map->get_texture(),
			.sampler = draw_cmd.model.material->albedo_map->get_sampler()
		};
		SDL_BindGPUFragmentSamplers(pass, 0, &texture_sampler_binding, 1);

		SDL_DrawGPUIndexedPrimitives(
			pass, draw_cmd.model.mesh->get_index_count(), 1, 0, 0, 0
		);
	}
}

}  // namespace lili
