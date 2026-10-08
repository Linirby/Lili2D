#pragma once

#include "lili2d/core/batch_item.hpp"
#include "lili2d/render/common/atlas_map.hpp"
#include "lili2d/render/gpu/gpu_mesh.hpp"
#include <deque>

namespace lili {

class ChunkBatch
{
public:
    /// @brief Constructor.
    /// @param renderer The renderer.
    /// @param texture The texture to use for the batch.
    explicit ChunkBatch(Renderer* renderer, Texture* texture);
    /// @brief Destructor.
    ~ChunkBatch() = default;

    /// @brief Move constructor.
    ChunkBatch(ChunkBatch&&) noexcept = default;
    /// @brief Move assignment operator.
    ChunkBatch&
    operator=(ChunkBatch&&) noexcept = default;

    /// @brief Deleted copy constructor.
    ChunkBatch(const ChunkBatch&) = delete;
    /// @brief Deleted copy assignment operator.
    ChunkBatch&
    operator=(const ChunkBatch&) = delete;

    /// @brief Begins a new batch, clearing previous data.
    inline void
    begin() noexcept
    {
        mesh_data.vertices.clear();
        mesh_data.indices.clear();
        pool_idx = 0;
    }

    /// @brief Adds a slice to the batch mesh.
    /// @param slice The texture slice (UVs) to use.
    /// @param position The local position.
    /// @param scale The local scale.
    /// @param rotation The local rotation in degrees.
    /// @param color The color tint for the vertices.
    void
    add(const SliceUV& slice,
        Vec2 position,
        Vec2 scale = { 1.0f, 1.0f },
        float rotation = 0.0f,
        Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f });

    void
    end();
    void
    draw();
    inline void
    setLayer(float layer) noexcept
    {
        this->layer = layer;
    }
    void
    setMeshData(MeshData&& data);
    void
    clear()
    {
        mesh_data.vertices.clear();
        mesh_data.indices.clear();
        pool_idx = 0;
        if (!batch_pool.empty() && batch_pool[0].mesh)
            batch_pool[0].mesh->update(mesh_data);
    }
    static void
    appendToMesh(
        MeshData& mesh_data,
        const SliceUV& slice,
        Vec2 position,
        Vec2 scale = { 1.0f, 1.0f },
        float rotation = 0.0f,
        Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }
    );

private:
    Renderer* renderer = nullptr;
    Texture* active_texture = nullptr;
    std::deque<BatchItem> batch_pool;
    size_t pool_idx = 0;
    MeshData mesh_data;
    float layer = 0.0f;
};

} // namespace lili
