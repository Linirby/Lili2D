#include "lili2d/render/gpu/gpu_mesh.hpp"

#include <SDL3/SDL_gpu.h>
#include <cstdint>
#include <cstring>
#include <stdexcept>

namespace lili {

GPUMesh::GPUMesh(
    SDL_GPUDevice* device,
    const MeshData& mesh,
    SDL_GPUCommandBuffer* cmd
)
  : device(device)
{
    index_count = static_cast<uint32_t>(mesh.indices.size());

    uint32_t vertex_buf_size = (mesh.vertices.size() * sizeof(lili::Vertex));
    uint32_t index_buf_size = mesh.indices.size() * sizeof(uint32_t);
    if (vertex_buf_size + index_buf_size == 0)
        return;

    if (vertex_buf_size > 0) {
        SDL_GPUBufferCreateInfo vertices_bci{};
        vertices_bci.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
        vertices_bci.size = vertex_buf_size;

        vertex_buffer = std::unique_ptr<SDL_GPUBuffer, SDLGPUBufferDeleter>(
            SDL_CreateGPUBuffer(this->device, &vertices_bci),
            SDLGPUBufferDeleter(this->device)
        );
        if (!vertex_buffer)
            throw std::runtime_error(
                "vertex_buffer creation failed!\n-> " +
                std::string(SDL_GetError())
            );
        vertex_capacity = vertex_buf_size;
    } else {
        vertex_buffer = nullptr;
        vertex_capacity = 0;
    }

    if (index_buf_size > 0) {
        SDL_GPUBufferCreateInfo indices_bci{};
        indices_bci.usage = SDL_GPU_BUFFERUSAGE_INDEX;
        indices_bci.size = index_buf_size;

        index_buffer = std::unique_ptr<SDL_GPUBuffer, SDLGPUBufferDeleter>(
            SDL_CreateGPUBuffer(this->device, &indices_bci),
            SDLGPUBufferDeleter(this->device)
        );
        if (!index_buffer)
            throw std::runtime_error(
                "index_buffer creation failed!\n-> " +
                std::string(SDL_GetError())
            );
        index_capacity = index_buf_size;
    } else {
        index_buffer = nullptr;
        index_capacity = 0;
    }

    transferToGpu(
        TransferData{ .v_data = mesh.vertices.data(),
                      .i_data = mesh.indices.data(),
                      .v_size = vertex_buf_size,
                      .i_size = index_buf_size },
        vertex_buffer.get(),
        index_buffer.get(),
        cmd
    );
}

void
GPUMesh::update(const MeshData& mesh, SDL_GPUCommandBuffer* cmd)
{
    index_count = static_cast<uint32_t>(mesh.indices.size());

    uint32_t vertex_buf_size = mesh.vertices.size() * sizeof(lili::Vertex);
    uint32_t index_buf_size = mesh.indices.size() * sizeof(uint32_t);
    if (vertex_buf_size + index_buf_size == 0)
        return;

    if (vertex_buf_size > vertex_capacity) {
        SDL_GPUBufferCreateInfo vertices_bci{};
        vertices_bci.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
        vertices_bci.size = vertex_buf_size;
        vertex_buffer = std::unique_ptr<SDL_GPUBuffer, SDLGPUBufferDeleter>(
            SDL_CreateGPUBuffer(device, &vertices_bci),
            SDLGPUBufferDeleter(device)
        );
        if (!vertex_buffer)
            throw std::runtime_error(
                "vertex_buffer creation failed in update!\n-> " +
                std::string(SDL_GetError())
            );
        vertex_capacity = vertex_buf_size;
    }

    if (index_buf_size > index_capacity) {
        SDL_GPUBufferCreateInfo indices_bci{};
        indices_bci.usage = SDL_GPU_BUFFERUSAGE_INDEX;
        indices_bci.size = index_buf_size;
        index_buffer = std::unique_ptr<SDL_GPUBuffer, SDLGPUBufferDeleter>(
            SDL_CreateGPUBuffer(device, &indices_bci),
            SDLGPUBufferDeleter(device)
        );
        if (!index_buffer)
            throw std::runtime_error(
                "index_buffer creation failed in update!\n-> " +
                std::string(SDL_GetError())
            );
        index_capacity = index_buf_size;
    }

    transferToGpu(
        TransferData{ .v_data = mesh.vertices.data(),
                      .i_data = mesh.indices.data(),
                      .v_size = vertex_buf_size,
                      .i_size = index_buf_size },
        vertex_buffer.get(),
        index_buffer.get(),
        cmd
    );
}

void
GPUMesh::transferToGpu(
    TransferData data,
    SDL_GPUBuffer* vertex_buffer,
    SDL_GPUBuffer* index_buffer,
    SDL_GPUCommandBuffer* cmd
)
{
    SDL_GPUTransferBufferCreateInfo tb_ci{};
    tb_ci.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    tb_ci.size = data.v_size + data.i_size;

    SDL_GPUTransferBuffer* tbuf = SDL_CreateGPUTransferBuffer(device, &tb_ci);
    void* map = SDL_MapGPUTransferBuffer(device, tbuf, false);
    if (!map) {
        SDL_ReleaseGPUTransferBuffer(device, tbuf);
        throw std::runtime_error(
            "Failed to map GPU transfer buffer: " + std::string(SDL_GetError())
        );
    }

    if (data.v_size > 0)
        std::memcpy(map, data.v_data, data.v_size);
    if (data.i_size > 0)
        std::memcpy(
            static_cast<char*>(map) + data.v_size, data.i_data, data.i_size
        );
    SDL_UnmapGPUTransferBuffer(device, tbuf);

    bool intern_cmd = (cmd == nullptr);
    if (intern_cmd)
        cmd = SDL_AcquireGPUCommandBuffer(device);
    SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(cmd);

    if (data.v_size > 0) {
        SDL_GPUTransferBufferLocation src_v{};
        src_v.transfer_buffer = tbuf;
        src_v.offset = 0;

        SDL_GPUBufferRegion dst_v{};
        dst_v.buffer = vertex_buffer;
        dst_v.offset = 0;
        dst_v.size = data.v_size;

        SDL_UploadToGPUBuffer(copy_pass, &src_v, &dst_v, false);
    }

    if (data.i_size > 0) {
        SDL_GPUTransferBufferLocation src_i{};
        src_i.transfer_buffer = tbuf;
        src_i.offset = data.v_size;

        SDL_GPUBufferRegion dst_i{};
        dst_i.buffer = index_buffer;
        dst_i.offset = 0;
        dst_i.size = data.i_size;

        SDL_UploadToGPUBuffer(copy_pass, &src_i, &dst_i, false);
    }

    SDL_EndGPUCopyPass(copy_pass);
    if (intern_cmd)
        SDL_SubmitGPUCommandBuffer(cmd);
    SDL_ReleaseGPUTransferBuffer(device, tbuf);
}

} // namespace lili
