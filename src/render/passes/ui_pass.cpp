#include "render/passes/ui_pass.hpp"

#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

#include "render/scene/model.hpp"

namespace lili {

static SDL_GPUBuffer *create_materials_buffer(SDL_GPUDevice *device) {
	const MaterialRegistry &registry = MaterialRegistry::get();
	const size_t material_count = registry.material_count();
	if (material_count == 0) {
		throw std::runtime_error("Material registry is empty.");
	}

	std::vector<MaterialGPU> gpu_materials;
	gpu_materials.reserve(material_count);
	for (size_t i = 0; i < material_count; ++i) {
		const MaterialProps &props = registry.get_material(
			static_cast<uint16_t>(i)
		).properties;
		gpu_materials.push_back({
			{
				props.color_tint.x,
				props.color_tint.y,
				props.color_tint.z,
				props.color_tint.w
			}
		});
	}

	const uint32_t size = static_cast<uint32_t>(
		gpu_materials.size() * sizeof(MaterialGPU)
	);
	SDL_GPUBufferCreateInfo materials_buffer_createinfo{
		.usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
		.size = size,
		.props = 0
	};
	SDL_GPUBuffer *materials_buffer = SDL_CreateGPUBuffer(
		device, &materials_buffer_createinfo
	);
	if (!materials_buffer) {
		throw std::runtime_error(
			"Materials storage buffer creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}

	SDL_GPUTransferBufferCreateInfo transfer_buffer_info{
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size = size,
		.props = 0
	};
	SDL_GPUTransferBuffer *transfer_buffer = SDL_CreateGPUTransferBuffer(
		device, &transfer_buffer_info
	);
	if (!transfer_buffer) {
		SDL_ReleaseGPUBuffer(device, materials_buffer);
		throw std::runtime_error(
			"Materials transfer buffer creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}

	void *map = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
	if (!map) {
		SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
		SDL_ReleaseGPUBuffer(device, materials_buffer);
		throw std::runtime_error(
			"Failed to map materials transfer buffer!\n-> " +
			std::string(SDL_GetError())
		);
	}
	std::memcpy(map, gpu_materials.data(), size);
	SDL_UnmapGPUTransferBuffer(device, transfer_buffer);

	SDL_GPUCommandBuffer *upload_cmd = SDL_AcquireGPUCommandBuffer(device);
	if (!upload_cmd) {
		SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
		SDL_ReleaseGPUBuffer(device, materials_buffer);
		throw std::runtime_error(
			"Failed to acquire command buffer for materials upload!"
		);
	}
	SDL_GPUCopyPass *copy_pass = SDL_BeginGPUCopyPass(upload_cmd);
	SDL_GPUTransferBufferLocation src{
		.transfer_buffer = transfer_buffer,
		.offset = 0
	};
	SDL_GPUBufferRegion dst{
		.buffer = materials_buffer,
		.offset = 0,
		.size = size
	};
	SDL_UploadToGPUBuffer(copy_pass, &src, &dst, false);
	SDL_EndGPUCopyPass(copy_pass);
	SDL_SubmitGPUCommandBuffer(upload_cmd);
	SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);

	return materials_buffer;
}

UIPass::UIPass(
	SDL_GPUDevice *device, SDL_GPUGraphicsPipeline *pipeline
) {
	this->device = device;
	this->pipeline = pipeline;
	this->materials_buffer = create_materials_buffer(device);
}

UIPass::~UIPass() {
	if (materials_buffer)
		SDL_ReleaseGPUBuffer(device, materials_buffer);
}

void UIPass::render(
	SDL_GPURenderPass *pass,
	SDL_GPUCommandBuffer *cmd,
	const Mat3 &proj_view,
	const std::vector<DrawCommand> &queue
) {
	if (queue.empty()) return;

	SDL_BindGPUGraphicsPipeline(pass, pipeline);
	SDL_BindGPUFragmentStorageBuffers(pass, 0, &materials_buffer, 1);

	for (const DrawCommand &draw_cmd : queue) {
		if (!draw_cmd.model)
			throw std::runtime_error(
				"UIPass received draw command without model."
			);
		if (!draw_cmd.model->mesh)
			throw std::runtime_error(
				"UIPass received draw command without mesh."
			);
		if (!draw_cmd.model->material)
			throw std::runtime_error(
				"UIPass received draw command without material."
			);
		if (!draw_cmd.model->material->albedo_map)
			throw std::runtime_error(
				"UIPass received draw command with material missing albedo map."
			);

		Mat3 mvp = proj_view * draw_cmd.transform;
		struct UIUniforms {
			float matrix[12];
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
		uniforms.layer = draw_cmd.layer;

		SDL_PushGPUVertexUniformData(cmd, 0, &uniforms, sizeof(uniforms));

		SDL_GPUBufferBinding vertex_binding{
			.buffer = draw_cmd.model->mesh->get_vertex(),
			.offset = 0
		};
		SDL_BindGPUVertexBuffers(pass, 0, &vertex_binding, 1);

		SDL_GPUBufferBinding index_binding{
			.buffer = draw_cmd.model->mesh->get_index(),
			.offset = 0
		};
		SDL_BindGPUIndexBuffer(
			pass, &index_binding, SDL_GPU_INDEXELEMENTSIZE_32BIT
		);

		SDL_GPUTextureSamplerBinding texture_sampler_binding{
			.texture = draw_cmd.model->material->albedo_map->get_texture(),
			.sampler = draw_cmd.model->material->albedo_map->get_sampler()
		};
		SDL_BindGPUFragmentSamplers(pass, 0, &texture_sampler_binding, 1);

		SDL_DrawGPUIndexedPrimitives(
			pass, draw_cmd.model->mesh->get_index_count(), 1, 0, 0, 0
		);
	}
}

}  // namespace lili
