#pragma once

#include <SDL3/SDL_gpu.h>
#include <vector>

namespace lili {

/**
 * \brief Represents a single vertex in a mesh.
 */
struct Vertex {
	float x, y, z;       ///< Position coordinates.
	float u, v;          ///< Texture coordinates.
	float material_id;   ///< Material ID.
};

/**
 * \brief Contains the CPU-side data for a mesh.
 */
struct MeshData {
	std::vector<Vertex> vertices;  ///< List of vertices.
	std::vector<uint32_t> indices; ///< List of indices.
};

/**
 * \brief Represents a mesh stored on the GPU.
 */
class GPUMesh {
public:
	/**
	 * \brief Constructs a GPU mesh from CPU data.
	 * \param device The SDL GPU device.
	 * \param mesh The CPU mesh data.
	 */
	GPUMesh(SDL_GPUDevice *device, const MeshData &mesh);
	/// \brief Destructor.
	~GPUMesh();

	/**
	 * \brief Gets the vertex buffer.
	 * \return Pointer to the vertex SDL_GPUBuffer.
	 */
	SDL_GPUBuffer *get_vertex() const;
	/**
	 * \brief Gets the index buffer.
	 * \return Pointer to the index SDL_GPUBuffer.
	 */
	SDL_GPUBuffer *get_index() const;
	/**
	 * \brief Gets the number of indices in the mesh.
	 * \return The index count.
	 */
	uint32_t get_index_count() const;

private:
	SDL_GPUDevice *device = nullptr;
	SDL_GPUBuffer *vertex_buffer = nullptr;
	SDL_GPUBuffer *index_buffer = nullptr;
	uint32_t index_count = 0;

	void transfer_to_gpu(
		const void *data, SDL_GPUBuffer *buffer, uint32_t size
	);
};

}  // namespace lili
