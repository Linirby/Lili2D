#include "lili2d/entities/circle.hpp"
#include "lili2d/components/circle.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/geometry/vec2.hpp"

namespace lili {

[[nodiscard]] Entity
createCircle(ECSRegistry& registry, const CircleParameters& params)
{
    Entity entity = registry.createEntity();

    cTransform transform = params.transform;
    transform.prev_pos = params.transform.pos;

    cRender2D render2d = params.render2d;
    render2d.size = Vec2{
        params.circle.radius * 2.0f,
        params.circle.radius * 2.0f,
    };

    registry.addComponents(entity, transform, render2d, params.circle);
    return entity;
}

} // namespace lili
