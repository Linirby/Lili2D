#pragma once

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/2d/dynamic_batch.hpp"
#include "lili2d/render/common/atlas_map.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include <cstdint>
#include <vector>

namespace lili {

struct DrawItem
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

class Render2DSystem
{
public:
    static void
    render(ECSRegistry& registry, DynamicBatch& batch)
    {
        renderSprites(registry, batch);
    }

private:
    inline static std::vector<DrawItem> items;

    static void
    renderSprites(ECSRegistry& registry, DynamicBatch& batch);
    static void
    renderShapes(ECSRegistry& registry, DynamicBatch& batch);
};

} // namespace lili
