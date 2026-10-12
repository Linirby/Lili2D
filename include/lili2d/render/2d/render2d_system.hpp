#pragma once

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/2d/dynamic_batch.hpp"
#include "lili2d/render/common/atlas_map.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include "lili2d/render/gpu/texture.hpp"
#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

namespace lili {

struct SpriteDrawItem
{
    Entity entity;
    RenderLayer render_pass;
    float layer;
    uint16_t material_id;
    SliceUV slice;
    Vec2 pos;
    Vec2 scale;
    float rotation;
    Vec4 tint;
};

struct RectDrawItem
{
    Entity entity;
    RenderLayer render_pass;
    float layer;
    uint16_t material_id;
    Vec2 pos;
    Vec2 size;
    Vec2 scale;
    float rotation;
    Vec4 color;
    bool hollow;
    float thickness;
};

struct CircleDrawItem
{
    Entity entity;
    RenderLayer render_pass;
    float layer;
    uint16_t material_id;
    Vec2 pos;
    float radius;
    size_t segments;
    Vec2 scale;
    Vec4 color;
    bool hollow;
    float thickness;
};

struct LineDrawItem
{
    Entity entity;
    RenderLayer render_pass;
    float layer;
    uint16_t material_id;
    Vec2 dxy;
    float thickness;
    Vec4 color;
};

class Render2DSystem
{
public:
    static void
    render(ECSRegistry& registry, DynamicBatch& batch, Texture* the_white_pixel)
    {
        batch.begin();
        renderSprites(registry, batch);
        renderRects(registry, batch, the_white_pixel);
        renderCircles(registry, batch, the_white_pixel);
        renderLines(registry, batch, the_white_pixel);
    }

private:
    inline static std::vector<SpriteDrawItem> sprites;
    inline static std::vector<uint32_t> sprite_indices;
    inline static std::vector<RectDrawItem> rects;
    inline static std::vector<uint32_t> rect_indices;

    inline static float current_layer = std::numeric_limits<float>::min();
    inline static RenderLayer current_render_pass = RenderLayer::UNDEFINE;
    inline static uint16_t current_material_id = 0;
    inline static Texture* current_texture = nullptr;

    static void
    renderSprites(ECSRegistry& registry, DynamicBatch& batch);
    static void
    renderRects(
        ECSRegistry& registry,
        DynamicBatch& batch,
        Texture* the_white_pixel
    );
    static void
    renderCircles(
        ECSRegistry& registry,
        DynamicBatch& batch,
        Texture* the_white_pixel
    );
    static void
    renderLines(
        ECSRegistry& registry,
        DynamicBatch& batch,
        Texture* the_white_pixel
    );
};

} // namespace lili
