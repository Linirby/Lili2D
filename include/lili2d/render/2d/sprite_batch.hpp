#pragma once

#include <cstddef>
#include <deque>
#include <memory>
#include <vector>

#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/common/atlas_map.hpp"
#include "lili2d/render/common/material.hpp"
#include "lili2d/render/common/renderable.hpp"
#include "lili2d/render/gpu/gpu_mesh.hpp"
#include "lili2d/render/gpu/texture.hpp"
#include "lili2d/render/renderer.hpp"

namespace lili {

struct BatchItem
{
    std::unique_ptr<GPUMesh> mesh;
    Material material;
};

/// @brief Batches multiple sprites into a single draw call.
class SpriteBatch
{
public:
    /// @brief Constructor.
    /// @param renderer The renderer.
    /// @param texture The texture to use for the batch.
    explicit SpriteBatch(Renderer* renderer, Texture* texture);
    /// @brief Destructor.
    ~SpriteBatch() = default;

    /// @brief Move constructor.
    SpriteBatch(SpriteBatch&&) noexcept = default;
    /// @brief Move assignment operator.
    SpriteBatch&
    operator=(SpriteBatch&&) noexcept = default;

    /// @brief Deleted copy constructor.
    SpriteBatch(const SpriteBatch&) = delete;
    /// @brief Deleted copy assignment operator.
    SpriteBatch&
    operator=(const SpriteBatch&) = delete;

    /// @brief Begins a new batch, clearing previous data.
    inline void
    begin() noexcept
    {
        mesh_data.vertices.clear();
        mesh_data.indices.clear();
        pool_idx = 0;
    }

    /// @brief Adds a sprite to the batch.
    /// @param slice The texture slice (UVs) to use.
    /// @param position The local position.
    /// @param scale The local scale.
    /// @param rotation The local rotation in degrees.
    /// @param color The color tint for the vertices.
    void
    draw(
        const SliceUV& slice,
        Vec2 position,
        Vec2 scale = { 1.0f, 1.0f },
        float rotation = 0.0f,
        Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }
    );

    /// @brief Submits to the renderer and flushes the batch.
    void
    flush(float layer, RenderLayer render_pass = RenderLayer::WORLD2D);

    // Temporary: Only for Tilemap that needs a rework
    /// @brief Ends the batch (kept for backwards compatibility).
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
    static inline void
    draw(
        MeshData& mesh_data,
        const SliceUV& slice,
        Vec2 position,
        Vec2 scale = { 1.0f, 1.0f },
        float rotation = 0.0f,
        Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }
    )
    {
        appendToMesh(mesh_data, slice, position, scale, rotation, color);
    }

private:
    Renderer* renderer = nullptr;
    Material* external_material = nullptr;
    Texture* active_texture = nullptr;

    std::deque<BatchItem> batch_pool;
    size_t pool_idx = 0;

    MeshData mesh_data;

    float layer = 0.0f; // Temporary: Only for Tilemap that needs a rework
};

} // namespace lili
