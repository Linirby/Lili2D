#pragma once

#include <SDL3/SDL_gpu.h>

#include <cstdint>
#include <memory>
#include <vector>

#include "lili2d/core/sdl_deleters.hpp"
#include "lili2d/render/gpu/vertex.hpp"

namespace lili {

/// @brief Contains the CPU-side data for a mesh.
struct MeshData
{
    std::vector<Vertex> vertices;  ///< List of vertices.
    std::vector<uint32_t> indices; ///< List of indices.
};

/// @brief Contains the data used to transfer a MeshData to the GPU
struct TransferData
{
    const void* v_data = nullptr;
    const void* i_data = nullptr;
    uint32_t v_size = 0; ///< Size of vertices.
    uint32_t i_size = 0; ///< Size of indices.
};

/// @brief Represents a mesh stored on the GPU.
class GPUMesh
{
public:
    /// @brief Constructs a GPU mesh from CPU data.
    /// @param device The SDL GPU device.
    /// @param mesh The CPU mesh data.
    explicit GPUMesh(
        SDL_GPUDevice* device,
        const MeshData& mesh,
        SDL_GPUCommandBuffer* cmd = nullptr
    );
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
    [[nodiscard]] inline SDL_GPUBuffer*
    getVertex() const noexcept
    {
        return vertex_buffer.get();
    }
    /// @brief Gets the index buffer.
    /// @return Pointer to the index SDL_GPUBuffer.
    [[nodiscard]] inline SDL_GPUBuffer*
    getIndex() const noexcept
    {
        return index_buffer.get();
    }
    /// @brief Gets the number of indices in the mesh.
    /// @return The index count.
    [[nodiscard]] inline uint32_t
    getIndexCount() const noexcept
    {
        return index_count;
    }

    /// @brief Updates the mesh data.
    /// @param mesh The new CPU mesh data.
    void
    update(const MeshData& mesh, SDL_GPUCommandBuffer* cmd = nullptr);

private:
    SDL_GPUDevice* device;
    std::unique_ptr<SDL_GPUBuffer, SDLGPUBufferDeleter> vertex_buffer;
    std::unique_ptr<SDL_GPUBuffer, SDLGPUBufferDeleter> index_buffer;
    uint32_t vertex_capacity = 0;
    uint32_t index_capacity = 0;
    uint32_t index_count = 0;

    void
    transferToGpu(
        TransferData data,
        SDL_GPUBuffer* vertex_buffer,
        SDL_GPUBuffer* index_buffer,
        SDL_GPUCommandBuffer* cmd
    );
};

} // namespace lili
