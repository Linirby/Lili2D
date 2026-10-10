#pragma once

#include <cstddef>
#include <deque>
#include <vector>

#include "lili2d/core/batch_item.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/common/atlas_map.hpp"
#include "lili2d/render/common/renderable.hpp"
#include "lili2d/render/gpu/gpu_mesh.hpp"
#include "lili2d/render/gpu/texture.hpp"
#include "lili2d/render/renderer.hpp"

namespace lili {

/// @brief Batches multiple 2D textured slices into dynamic draw calls per
/// frame.
class DynamicBatch
{
public:
    /// @brief Constructor.
    /// @param renderer The renderer.
    /// @param texture The texture to use for the batch.
    explicit DynamicBatch(Renderer* renderer, Texture* texture);
    /// @brief Destructor.
    ~DynamicBatch() = default;

    /// @brief Move constructor.
    DynamicBatch(DynamicBatch&&) noexcept = default;
    /// @brief Move assignment operator.
    DynamicBatch&
    operator=(DynamicBatch&&) noexcept = default;

    /// @brief Deleted copy constructor.
    DynamicBatch(const DynamicBatch&) = delete;
    /// @brief Deleted copy assignment operator.
    DynamicBatch&
    operator=(const DynamicBatch&) = delete;

    /// @brief Begins a new batch, clearing previous data.
    inline void
    begin() noexcept
    {
        mesh_data.vertices.clear();
        mesh_data.indices.clear();
        pool_idx = 0;
    }

    /// @brief Adds a sprite slice to the batch mesh.
    /// @param slice The texture slice (UVs) to use.
    /// @param position The local position.
    /// @param scale The local scale.
    /// @param rotation The local rotation in degrees.
    /// @param tint The color tint for the vertices.
    void
    addSprite(
        const SliceUV& slice,
        Vec2 position,
        Vec2 scale,
        float rotation,
        Vec4 tint
    );

    /// @brief Adds a rect to the batch mesh.
    /// @param position The local position.
    /// @param size The size of the rect.
    /// @param scale The local scale.
    /// @param rotation The local rotation in degrees.
    /// @param tint The color for the rect vertices.
    void
    addRect(
        Vec2 position,
        Vec2 size,
        Vec2 scale,
        float rotation,
        Vec4 color,
        bool hollow,
        float thickness
    );

    /// @brief Submits to the renderer and flushes the batch.
    void
    flush(float layer, RenderLayer render_pass);

private:
    Renderer* renderer = nullptr;
    Texture* active_texture = nullptr;
    std::deque<BatchItem> batch_pool;
    size_t pool_idx = 0;
    MeshData mesh_data;
};

} // namespace lili
