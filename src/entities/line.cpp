#include "lili2d/entities/line.hpp"
#include "lili2d/components/line.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/geometry/vec2.hpp"
#include <cstdlib>

namespace lili {

[[nodiscard]] Entity
createLine(ECSRegistry& registry, const LineParameters& params)
{
    Entity entity = registry.createEntity();

    Render2DComponent render2d = params.render2d;
    render2d.size = Vec2{
        std::abs(params.line.dxy.x),
        std::abs(params.line.dxy.y),
    };

    LineComponent line = params.line;
    line.dxy = Vec2{
        params.line.dxy.x * params.transform.scale.x,
        params.line.dxy.y * params.transform.scale.y,
    };

    registry.addComponents(entity, params.transform, render2d, line);
    return entity;
}

} // namespace lili
