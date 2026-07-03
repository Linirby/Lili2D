#include "systems.hpp"
#include "components.hpp"

namespace systems {

void updateMovement(
	lili::ECSRegistry &registry, float dt, float window_w, float window_h
) {
	auto &pos_pool = registry.getPool<PositionComponent>();
	const auto &entities = pos_pool.getEntities();
	auto &positions = pos_pool.getComponents();

	for (size_t i = 0; i < entities.size(); ++i) {
		lili::Entity entity = entities[i];
		if (registry.hasComponent<VelocityComponent>(entity)) {
			auto &pos = positions[i];
			auto &vel = registry.getComponent<VelocityComponent>(entity);

			pos.value += vel.value * dt;

			if (registry.hasComponent<RenderComponent>(entity)) {
				auto &render = registry.getComponent<RenderComponent>(entity);
				float radius = render.circle.getRadius();

				if (pos.value.x - radius < 0.0f) {
					pos.value.x = radius;
					vel.value.x = -vel.value.x;
				} else if (pos.value.x + radius > window_w) {
					pos.value.x = window_w - radius;
					vel.value.x = -vel.value.x;
				}

				// Bounce off top/bottom
				if (pos.value.y - radius < 0.0f) {
					pos.value.y = radius;
					vel.value.y = -vel.value.y;
				} else if (pos.value.y + radius > window_h) {
					pos.value.y = window_h - radius;
					vel.value.y = -vel.value.y;
				}
			}
		}
	}
}

void renderEntities(lili::ECSRegistry &registry) {
	auto &pos_pool = registry.getPool<PositionComponent>();
	const auto &entities = pos_pool.getEntities();
	const auto &positions = pos_pool.getComponents();

	for (size_t i = 0; i < entities.size(); ++i) {
		lili::Entity entity = entities[i];
		if (registry.hasComponent<RenderComponent>(entity)) {
			const auto &pos = positions[i];
			auto &render = registry.getComponent<RenderComponent>(entity);

			render.circle.setCenter(pos.value);
			render.circle.draw();
		}
	}
}

} // namespace systems
