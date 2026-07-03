#include "entities.hpp"
#include "components.hpp"

namespace entities {

lili::Entity spawnBall(
	lili::ECSRegistry &registry,
	lili::Renderer *renderer,
	lili::Vec2 pos,
	lili::Vec2 vel,
	float radius,
	lili::Vec4 color
) {
	lili::Entity entity = registry.createEntity();
	registry.emplaceComponent<PositionComponent>(entity, pos);
	registry.emplaceComponent<VelocityComponent>(entity, vel);
	registry.emplaceComponent<RenderComponent>(
		entity, renderer, pos, radius, color
	);
	return entity;
}

} // namespace entities
