#pragma once

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/2d/dynamic_batch.hpp"
#include "lili2d/render/common/atlas_map.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include "lili2d/render/gpu/texture.hpp"
#include <cstdint>
#include <vector>

namespace lili {

struct SpriteDrawItem
{
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

class Render2DSystem
{
public:
    static void
    render(ECSRegistry& registry, DynamicBatch& batch, Texture* the_white_pixel)
    {
        renderSprites(registry, batch);
        renderShapes(registry, batch, the_white_pixel);
    }

private:
    inline static std::vector<SpriteDrawItem> sprites;
    inline static std::vector<RectDrawItem> rects;

    static void
    renderSprites(ECSRegistry& registry, DynamicBatch& batch);
    static void
    renderShapes(
        ECSRegistry& registry,
        DynamicBatch& batch,
        Texture* the_white_pixel
    );
};

} // namespace lili
