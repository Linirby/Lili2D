#include "render/passes/world_2d_pass.hpp"

#include <stdexcept>
#include <vector>
#include <SDL3/SDL_timer.h>

#include "render/scene/common/model.hpp"

namespace lili {

World2DPass::World2DPass(
	SDL_GPUDevice *device, SDL_GPUGraphicsPipeline *pipeline
) : device(device), pipeline(pipeline) {}

void World2DPass::render(
	SDL_GPURenderPass *pass,
	SDL_GPUCommandBuffer *cmd,
	const Mat3 &proj_view,
	const std::map<float, std::vector<DrawCommand>> &queue
) {
	if (queue.empty()) return;

	SDL_GPUGraphicsPipeline *current_pipeline = pipeline;
	SDL_BindGPUGraphicsPipeline(pass, current_pipeline);
	for (auto& pair : queue) {
		for (const DrawCommand &draw_cmd : pair.second) {

		if (!draw_cmd.model.mesh)
			throw std::runtime_error(
				"World2DPass received draw command without mesh."
			);
		if (!draw_cmd.model.material)
			throw std::runtime_error(
				"World2DPass received draw command without material."
			);
		if (!draw_cmd.model.material->albedoMap)
			throw std::runtime_error(
				"World2DPass received draw command with material missing "
				"albedo map."
			);

		SDL_GPUGraphicsPipeline *target_pipeline = (
			draw_cmd.model.material->custom_pipeline
		);
		if (!target_pipeline) target_pipeline = pipeline;
		if (target_pipeline != current_pipeline) {
			SDL_BindGPUGraphicsPipeline(pass, target_pipeline);
			current_pipeline = target_pipeline;
		}

		Mat3 mvp = proj_view * draw_cmd.transform;
		struct UIUniforms {
			float matrix[12];
			float color[4];
			float layer;
			float time;
			float padding[2];
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
		uniforms.time = SDL_GetTicks() / 1000.0f;

		SDL_PushGPUVertexUniformData(cmd, 0, &uniforms, sizeof(uniforms));

		if (!draw_cmd.model.material->custom_vertex_uniforms.empty()) {
			SDL_PushGPUVertexUniformData(
				cmd, 1,
				draw_cmd.model.material->custom_vertex_uniforms.data(),
				draw_cmd.model.material->custom_vertex_uniforms.size()
			);
		}
		if (!draw_cmd.model.material->custom_fragment_uniforms.empty()) {
			SDL_PushGPUFragmentUniformData(
				cmd, 0,
				draw_cmd.model.material->custom_fragment_uniforms.data(),
				draw_cmd.model.material->custom_fragment_uniforms.size()
			);
		}

		SDL_GPUBufferBinding vertex_binding{};
		vertex_binding.buffer = draw_cmd.model.mesh->getVertex();
		vertex_binding.offset = 0;

		SDL_BindGPUVertexBuffers(pass, 0, &vertex_binding, 1);

		SDL_GPUBufferBinding index_binding{};
		index_binding.buffer = draw_cmd.model.mesh->getIndex();
		index_binding.offset = 0;

		SDL_BindGPUIndexBuffer(
			pass, &index_binding, SDL_GPU_INDEXELEMENTSIZE_32BIT
		);

		SDL_GPUTextureSamplerBinding texture_sb{};
		texture_sb.texture = draw_cmd.model.material->albedoMap->getTexture();
		texture_sb.sampler = draw_cmd.model.material->albedoMap->getSampler();

		SDL_BindGPUFragmentSamplers(pass, 0, &texture_sb, 1);

		SDL_DrawGPUIndexedPrimitives(
			pass, draw_cmd.model.mesh->getIndexCount(), 1, 0, 0, 0
		);
		}
	}
}

}  // namespace lili
