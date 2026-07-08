#include "entities.hpp"
#include "components.hpp"

namespace entities {

lili::Entity spawnBall(
	lili::ECSRegistry &registry,
	lili::Vec2 pos,
	lili::Vec2 vel,
	lili::SliceUV slice,
	lili::Vec4 color,
	float radius
) {
	lili::Entity entity = registry.createEntity();
	registry.emplaceComponent<PositionComponent>(entity, pos);
	registry.emplaceComponent<VelocityComponent>(entity, vel);
	registry.emplaceComponent<RenderComponent>(entity, slice, color, radius);
	return entity;
}

}  // namespace entities
