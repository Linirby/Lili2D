#include "entities.hpp"

#include "components.hpp"

using namespace lili;

namespace entities {

Entity
spawnBall(
    ECSRegistry& registry,
    Vec2 pos,
    Vec2 vel,
    const SliceUV& slice,
    Vec4 color,
    float radius
)
{
    Entity entity = registry.createEntity();
    registry.emplaceComponent<PositionComponent>(entity, pos);
    registry.emplaceComponent<VelocityComponent>(entity, vel);
    registry.emplaceComponent<RenderComponent>(entity, slice, color, radius);
    return entity;
}

} // namespace entities
