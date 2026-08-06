#pragma once

#include <SDL3/SDL_gpu.h>

#include <memory>
#include <vector>

#include "lili2d/core/sdl_deleters.hpp"

namespace lili {

/// @brief Represents a single vertex in a mesh.
struct Vertex {
    float x = 0.0f;            ///< X position.
    float y = 0.0f;            ///< Y position.
    float z = 0.0f;            ///< Z position.
    float u = 0.0f;            ///< U texture coordinate.
    float v = 0.0f;            ///< V texture coordinate.
    float material_id = 0.0f;  ///< Material ID.
    float r = 1.0f;            ///< Red color.
    float g = 1.0f;            ///< Green color.
    float b = 1.0f;            ///< Blue color.
    float a = 1.0f;            ///< Alpha color.

    /// @brief Default constructor.
    Vertex() = default;
    /// @brief Constructs a Vertex with position, UV, material ID, and RGBA
    /// color.
    /// @param x X position.
    /// @param y Y position.
    /// @param z Z position.
    /// @param u U texture coordinate.
    /// @param v V texture coordinate.
    /// @param material_id Material ID.
    /// @param r Red color component.
    /// @param g Green color component.
    /// @param b Blue color component.
    /// @param a Alpha color component.
    Vertex(
        float x, float y, float z, float u = 0.0f, float v = 0.0f,
        float material_id = 0.0f, float r = 1.0f, float g = 1.0f,
        float b = 1.0f, float a = 1.0f
    );
};

/// @brief Contains the CPU-side data for a mesh.
struct MeshData {
    std::vector<Vertex> vertices;   ///< List of vertices.
    std::vector<uint32_t> indices;  ///< List of indices.
};

/// @brief Represents a mesh stored on the GPU.
class GPUMesh {
public:
    /// @brief Constructs a GPU mesh from CPU data.
    /// @param device The SDL GPU device.
    /// @param mesh The CPU mesh data.
    GPUMesh(SDL_GPUDevice* device, const MeshData& mesh);
    /// @brief Destructor.
    ~GPUMesh() = default;

    /// @brief Move constructor.
    GPUMesh(GPUMesh&& other) noexcept = default;
    /// @brief Move assignment operator.
    /// @param other Mesh instance to move from.
    /// @return Reference to this GPUMesh.
    GPUMesh&
    operator=(GPUMesh&& other) noexcept = default;

    /// @brief Copy constructor is deleted to prevent double-freeing GPU
    /// memory.
    GPUMesh(const GPUMesh&) = delete;
    /// @brief Copy assignment is deleted to prevent double-freeing GPU memory.
    GPUMesh&
    operator=(const GPUMesh&) = delete;

    /// @brief Gets the vertex buffer.
    /// @return Pointer to the vertex SDL_GPUBuffer.
    SDL_GPUBuffer*
    getVertex() const;
    /// @brief Gets the index buffer.
    /// @return Pointer to the index SDL_GPUBuffer.
    SDL_GPUBuffer*
    getIndex() const;
    /// @brief Gets the number of indices in the mesh.
    /// @return The index count.
    uint32_t
    getIndexCount() const;

    /// @brief Updates the mesh data.
    /// @param mesh The new CPU mesh data.
    void
    update(const MeshData& mesh);

private:
    /// @brief Pointer to the parent SDL_GPUDevice.
    SDL_GPUDevice* device = nullptr;
    /// @brief Unique pointer to vertex buffer.
    std::unique_ptr<SDL_GPUBuffer, SDLGPUBufferDeleter> vertex_buffer;
    /// @brief Unique pointer to index buffer.
    std::unique_ptr<SDL_GPUBuffer, SDLGPUBufferDeleter> index_buffer;
    /// @brief Total index count.
    uint32_t index_count = 0;
    /// @brief Total allocated vertex capacity.
    uint32_t vertex_capacity = 0;
    /// @brief Total allocated index capacity.
    uint32_t index_capacity = 0;

    /// @brief Helper to transfer data to the GPU buffer.
    /// @param data Pointer to the host data.
    /// @param buffer Pointer to the GPU buffer.
    /// @param size The size of the data in bytes.
    void
    transferToGpu(const void* data, SDL_GPUBuffer* buffer, uint32_t size);
};

}  // namespace lili
