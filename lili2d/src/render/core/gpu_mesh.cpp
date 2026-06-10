#include <cstring>
#include <stdexcept>

#include "render/core/gpu_mesh.hpp"

namespace lili {

GPUMesh::GPUMesh(SDL_GPUDevice *device, const MeshData &mesh)
	: device(device) {
	index_count = static_cast<uint32_t>(mesh.indices.size());

	uint32_t vertices_buffer_size = (
		mesh.vertices.size() * sizeof(lili::Vertex)
	);
	SDL_GPUBufferCreateInfo vertices_bci{};
	vertices_bci.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
	vertices_bci.size = vertices_buffer_size;

	vertex_buffer = SDL_CreateGPUBuffer(
		this->device, &vertices_bci
	);
	if (!vertex_buffer) {
		throw std::runtime_error(
			"vertex_buffer creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	transferToGpu(mesh.vertices.data(), vertex_buffer, vertices_buffer_size);

	uint32_t indices_buffer_size = mesh.indices.size() * sizeof(uint32_t);
	SDL_GPUBufferCreateInfo indices_bci{};
	indices_bci.usage = SDL_GPU_BUFFERUSAGE_INDEX;
	indices_bci.size = indices_buffer_size;

	index_buffer = SDL_CreateGPUBuffer(
		this->device, &indices_bci
	);
	if (!index_buffer) {
		throw std::runtime_error(
			"index_buffer creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	transferToGpu(
		mesh.indices.data(),
		index_buffer,
		indices_buffer_size
	);
}

GPUMesh::~GPUMesh() {
	if (index_buffer)
		SDL_ReleaseGPUBuffer(device, index_buffer);
	if (vertex_buffer)
		SDL_ReleaseGPUBuffer(device, vertex_buffer);
}

GPUMesh::GPUMesh(GPUMesh &&other) noexcept
	: device(other.device),
			vertex_buffer(other.vertex_buffer),
			index_buffer(other.index_buffer),
			index_count(other.index_count) {
	other.vertex_buffer = nullptr;
	other.index_buffer = nullptr;
	other.index_count = 0;
}

GPUMesh& GPUMesh::operator=(GPUMesh &&other) noexcept {
	if (this != &other) {
		if (index_buffer)
			SDL_ReleaseGPUBuffer(device, index_buffer);
		if (vertex_buffer)
			SDL_ReleaseGPUBuffer(device, vertex_buffer);

		device = other.device;
		vertex_buffer = other.vertex_buffer;
		index_buffer = other.index_buffer;
		index_count = other.index_count;

		other.vertex_buffer = nullptr;
		other.index_buffer = nullptr;
		other.index_count = 0;
	}
	return *this;
}

SDL_GPUBuffer *GPUMesh::getVertex() const {
	return vertex_buffer;
}

SDL_GPUBuffer *GPUMesh::getIndex() const {
	return index_buffer;
}

uint32_t GPUMesh::getIndexCount() const {
	return index_count;
}

void GPUMesh::transferToGpu(
	const void *data, SDL_GPUBuffer *buffer, uint32_t size
) {
	SDL_GPUTransferBufferCreateInfo transfer_bi{};
	transfer_bi.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
	transfer_bi.size = size;

	SDL_GPUTransferBuffer *transfer_buffer = SDL_CreateGPUTransferBuffer(
		device, &transfer_bi
	);
	void *map = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
	std::memcpy(map, data, size);
	SDL_UnmapGPUTransferBuffer(device, transfer_buffer);
	SDL_GPUCommandBuffer *upload_cmd = SDL_AcquireGPUCommandBuffer(device);
	SDL_GPUCopyPass *copy_pass = SDL_BeginGPUCopyPass(upload_cmd);
	SDL_GPUTransferBufferLocation src{};
	src.transfer_buffer = transfer_buffer;
	src.offset = 0;

	SDL_GPUBufferRegion dst{};
	dst.buffer = buffer;
	dst.offset = 0;
	dst.size = size;

	SDL_UploadToGPUBuffer(copy_pass, &src, &dst, false);
	SDL_EndGPUCopyPass(copy_pass);
	SDL_SubmitGPUCommandBuffer(upload_cmd);
	SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
}

}  // namespace lili
