#pragma once

#include <cstddef>
#include <cstdint>
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
    /// @param color The color for the rect vertices.
    /// @param hollow Boolean to say if the rect should be hollow
    /// @param thickness The border thickness when hollow
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

    /// @brief Adds a circle to the batch mesh.
    /// @param center_pos The local position.
    /// @param radius The radius of the circle.
    /// @param segments The number of segments that will compose the circle.
    /// @param color The color of the circle.
    /// @param hollow Boolean to say if the rect should be hollow
    /// @param thickness The border thickness when hollow
    void
    addCircle(
        Vec2 center_pos,
        float radius,
        float segments,
        Vec4 color,
        bool hollow,
        float thickness
    );

    /// @brief Adds a line to the batch mesh.
    /// @param start_pos The local start position.
    /// @param end_pos The local end position.
    /// @param thickness The border thickness when hollow
    /// @param color The color of the line.
    void
    addLine(Vec2 start_pos, Vec2 end_pos, float thickness, Vec4 color);

    /// @brief Adds text to the batch mesh.
    /// @param font The font.
    /// @param text The text.
    /// @param pos The center position.
    /// @param scale The local scale.
    /// @param rotation The local rotation in degrees.
    /// @param spacing The spacing between letters.
    /// @param color The color of the text.
    void
    addText(
        BitmapFont* font,
        std::string_view text,
        Vec2 pos,
        Vec2 scale,
        float rotation,
        float spacing,
        Vec4 color
    );

    /// @brief Submits to the renderer and flushes the batch.
    /// @param layer The layer to flush.
    /// @param render_pass The render pass to flush.
    /// @param material_id The material ID to look up in MaterialRegistry.
    void
    flush(float layer, RenderLayer render_pass, uint16_t material_id = 0);

private:
    Renderer* renderer = nullptr;
    Texture* active_texture = nullptr;
    std::deque<BatchItem> batch_pool;
    size_t pool_idx = 0;
    MeshData mesh_data;
};

} // namespace lili
