#pragma once

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/2d/bitmap_font.hpp"
#include "lili2d/render/2d/dynamic_batch.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include "lili2d/render/gpu/texture.hpp"
#include <cstdint>
#include <limits>
#include <type_traits>
#include <vector>

namespace lili {

enum class DrawType : uint8_t
{
    SPRITE,
    RECT,
    CIRCLE,
    LINE,
    TEXT
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
        } sprite;
        struct Rect
        {
            Vec2 scale;      ///< The scale
            float thickness; ///< The thickness (when hollow)
            bool hollow;     ///< The hollow indicator (true/false)
        } rect;
        struct Circle
        {
            float radius;      ///< The radius
            float thickness;   ///< The thickness (when hollow)
            uint32_t segments; ///< The segments (how detailed the circle is?)
            bool hollow;       ///< The hollow indicator (true/false)
        } circle;
        struct Line
        {
            Vec2 dxy;        ///< The distance X/Y of the end from the start
            float thickness; ///< The thickness
        } line;
        struct Text
        {
            BitmapFont* font; ///< The font
            Vec2 scale;       ///< The scale
            uint32_t offset;  ///< The offset in StringArena vector<char>
            uint32_t length;  ///< The length of the string (eg. "cat" = 3)
            float spacing;    ///< The spacing between letters
        } text;
    } nature;
};

static_assert(
    std::is_trivially_copyable_v<DrawItem>,
    "DrawItem must be trivially copyable."
);

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
    inline static std::vector<DrawItem> items;
    inline static std::vector<uint32_t> item_indices;

    inline static float current_layer = std::numeric_limits<float>::min();
    inline static RenderLayer current_render_pass = RenderLayer::UNDEFINE;
    inline static uint16_t current_material_id = 0;
    inline static Texture* current_texture = nullptr;

    static void
    pushSprites(ECSRegistry& registry);
    static void
    pushRects(ECSRegistry& registry, Texture* the_white_pixel);
    static void
    pushCircles(ECSRegistry& registry, Texture* the_white_pixel);
    static void
    pushLines(ECSRegistry& registry, Texture* the_white_pixel);
    static void
    pushTexts(ECSRegistry& registry);

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
        if (item_a.entity != item_b.entity)
            return item_a.entity < item_b.entity;
        return item_a.texture < item_b.texture;
    }

    static bool
    shouldFlush(const DrawItem& item)
    {
        return current_render_pass != item.render_pass ||
               current_layer != item.layer ||
               current_material_id != item.material_id ||
               current_texture != item.texture;
    }
};

} // namespace lili
