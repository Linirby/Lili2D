#pragma once

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/2d/dynamic_batch.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include "lili2d/render/gpu/texture.hpp"
#include <cstdint>
#include <limits>
#include <vector>

namespace lili {

// struct SpriteDrawItem
// {
//     Entity entity;
//     RenderLayer render_pass;
//     float layer;
//     uint16_t material_id;
//     SliceUV slice;
//     Vec2 pos;
//     Vec2 scale;
//     float rotation;
//     Vec4 tint;
// };

// struct RectDrawItem
// {
//     Entity entity;
//     RenderLayer render_pass;
//     float layer;
//     uint16_t material_id;
//     Vec2 pos;
//     Vec2 size;
//     Vec2 scale;
//     float rotation;
//     Vec4 color;
//     bool hollow;
//     float thickness;
// };

// struct CircleDrawItem
// {
//     Entity entity;
//     RenderLayer render_pass;
//     float layer;
//     uint16_t material_id;
//     Vec2 pos;
//     float radius;
//     size_t segments;
//     Vec4 color;
//     bool hollow;
//     float thickness;
// };

// struct LineDrawItem
// {
//     Entity entity;
//     RenderLayer render_pass;
//     float layer;
//     uint16_t material_id;
//     Vec2 start_pos;
//     Vec2 dxy;
//     float thickness;
//     Vec4 color;
// };

enum class DrawType : uint8_t
{
    SPRITE,
    RECT,
    CIRCLE,
    LINE
};

struct DrawItem
{
    Texture* texture;        ///< The texture
    Vec4 color;              ///< The color/tint
    Vec2 pos;                ///< The position
    Vec2 size;               ///< The size
    float layer;             ///< The layer/depth
    float rotation;          ///< The rotation
    Entity entity;           ///< The entity (the ID)
    uint16_t material_id;    ///< The material ID
    RenderLayer render_pass; ///< The render pass (WORLD2D / UI / ...)
    DrawType type;           ///< The type of the item

    union Nature
    {
        struct Sprite
        {
            Vec2 scale;  ///< The scale
            float u_min; ///< The minimum U of the UV
            float v_min; ///< The minimum V of the UV
            float u_max; ///< The maximum U of the UV
            float v_max; ///< The maximum V of the UV
        } sprite;        ///< The sprite struct
        struct
        {
            Vec2 scale;      ///< The scale
            float thickness; ///< The thickness (when hollow)
            bool hollow;     ///< The hollow indicator (true/false)
        } rect;              ///< The rect struct
        struct
        {
            float radius;      ///< The radius
            float thickness;   ///< The thickness (when hollow)
            uint32_t segments; ///< The segments (how detailed the circle is?)
            bool hollow;       ///< The hollow indicator (true/false)
        } circle;              ///< The circle struct
        struct
        {
            Vec2 dxy;        ///< The distance X/Y of the end from the start.
            float thickness; ///< The thickness
        } line;              ///< The line struct
    } nature;
};

class Render2DSystem
{
public:
    static void
    render(
        ECSRegistry& registry,
        DynamicBatch& batch,
        Texture* the_white_pixel
    );

private:
    // inline static std::vector<SpriteDrawItem> sprites;
    // inline static std::vector<uint32_t> sprite_indices;
    // inline static std::vector<RectDrawItem> rects;
    // inline static std::vector<uint32_t> rect_indices;
    // inline static std::vector<CircleDrawItem> circles;
    // inline static std::vector<uint32_t> circle_indices;
    // inline static std::vector<LineDrawItem> lines;
    // inline static std::vector<uint32_t> line_indices;
    inline static std::vector<DrawItem> items;
    inline static std::vector<uint32_t> item_indices;

    inline static float current_layer = std::numeric_limits<float>::min();
    inline static RenderLayer current_render_pass = RenderLayer::UNDEFINE;
    inline static uint16_t current_material_id = 0;
    inline static Texture* current_texture = nullptr;

    static void
    pushSprites(ECSRegistry& registry, DynamicBatch& batch);
    static void
    pushRects(
        ECSRegistry& registry,
        DynamicBatch& batch,
        Texture* the_white_pixel
    );
    static void
    pushCircles(
        ECSRegistry& registry,
        DynamicBatch& batch,
        Texture* the_white_pixel
    );
    static void
    pushLines(
        ECSRegistry& registry,
        DynamicBatch& batch,
        Texture* the_white_pixel
    );

    static bool
    compareIndex(uint32_t idx_a, uint32_t idx_b)
    {
        const DrawItem& item_a = items[idx_a];
        const DrawItem& item_b = items[idx_b];

        if (item_a.render_pass != item_b.render_pass)
            return item_a.render_pass < item_b.render_pass;
        if (item_a.layer != item_b.layer)
            return item_a.layer < item_b.layer;
        if (item_a.material_id != item_b.material_id)
            return item_a.material_id < item_b.material_id;
        if (item_a.texture != item_b.texture)
            return item_a.texture < item_b.texture;
        return item_a.entity < item_b.entity;
    }

    static bool
    shouldFlush(DrawItem item)
    {
        return current_render_pass != item.render_pass ||
               current_layer != item.layer ||
               current_material_id != item.material_id ||
               current_texture != item.texture;
    }
};

} // namespace lili
