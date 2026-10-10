#pragma once

#include <memory>
#include <vector>

#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/common/atlas_map.hpp"
#include "lili2d/render/common/material.hpp"
#include "lili2d/render/common/renderable.hpp"
#include "lili2d/render/gpu/gpu_mesh.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include "lili2d/render/gpu/texture.hpp"
#include "lili2d/render/renderer.hpp"

namespace lili {

/// @brief Batches multiple 2D textured slices into a static GPU mesh baked once
/// in VRAM.
class StaticBatch
{
public:
    /// @brief Constructor.
    /// @param renderer The renderer.
    /// @param texture The texture to use for the static batch.
    explicit StaticBatch(Renderer* renderer, Texture* texture);
    /// @brief Destructor.
    ~StaticBatch() = default;

    /// @brief Move constructor.
    StaticBatch(StaticBatch&&) noexcept = default;
    /// @brief Move assignment operator.
    StaticBatch&
    operator=(StaticBatch&&) noexcept = default;

    /// @brief Deleted copy constructor.
    StaticBatch(const StaticBatch&) = delete;
    /// @brief Deleted copy assignment operator.
    StaticBatch&
    operator=(const StaticBatch&) = delete;

    /// @brief Begins or clears CPU mesh staging data.
    inline void
    begin() noexcept
    {
        mesh_data.vertices.clear();
        mesh_data.indices.clear();
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
    addRect(Vec2 position, Vec2 size, Vec2 scale, float rotation, Vec4 color);

    /// @brief Bakes all added sprites into a GPUMesh in VRAM and clears CPU
    /// staging memory.
    void
    bake();

    /// @brief Submits the already baked GPUMesh to the renderer.
    /// @param layer Draw layer depth for sorting.
    /// @param render_pass Render pass target (WORLD2D or UI).
    void
    draw(float layer, RenderLayer render_pass);

    /// @brief Clears both GPU mesh and CPU staging data.
    void
    clear();

private:
    Renderer* renderer = nullptr;
    Texture* active_texture = nullptr;
    std::unique_ptr<GPUMesh> mesh;
    Material material;
    MeshData mesh_data;
};

} // namespace lili
