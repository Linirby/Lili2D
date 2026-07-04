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
}

void renderEntities(lili::ECSRegistry &registry, lili::SpriteBatch &batch) {
	batch.begin();

	auto &pos_pool = registry.getPool<PositionComponent>();
	const auto &entities = pos_pool.getEntities();
	const auto &positions = pos_pool.getComponents();

	for (size_t i = 0; i < entities.size(); ++i) {
		lili::Entity entity = entities[i];
		if (registry.hasComponent<RenderComponent>(entity)) {
			const auto &pos = positions[i];
			auto &render = registry.getComponent<RenderComponent>(entity);

			batch.draw(
				render.slice,
				pos.value,
				{ render.radius * 2.0f, render.radius * 2.0f },
				0.0f,
				render.color
			);
		}
	}

	batch.end();
	batch.draw();
}

}  // namespace systems
