#include "systems.hpp"
#include "components.hpp"

namespace systems {

MovementSystem::MovementSystem(float window_w, float window_h)
	: window_w(window_w), window_h(window_h) {
	parallel_threshold = 500;
}

void MovementSystem::updateEntity(
	lili::ECSRegistry& registry,
	lili::Entity entity,
	PositionComponent& pos,
	float dt
) {
	if (registry.hasComponent<VelocityComponent>(entity)) {
		auto& vel = registry.getComponent<VelocityComponent>(entity);

		pos.value += vel.value * dt;

		if (registry.hasComponent<RenderComponent>(entity)) {
			auto& render = registry.getComponent<RenderComponent>(entity);

			if (pos.value.x - render.radius < 0.0f) {
				pos.value.x = render.radius;
				vel.value.x = -vel.value.x;
			} else if (pos.value.x + render.radius > window_w) {
				pos.value.x = window_w - render.radius;
				vel.value.x = -vel.value.x;
			}

			if (pos.value.y - render.radius < 0.0f) {
				pos.value.y = render.radius;
				vel.value.y = -vel.value.y;
			} else if (pos.value.y + render.radius > window_h) {
				pos.value.y = window_h - render.radius;
				vel.value.y = -vel.value.y;
			}
		}
	}
}

} // namespace systems
