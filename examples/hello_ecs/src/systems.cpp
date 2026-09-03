#include "systems.hpp"

#include <tuple>

#include "components.hpp"
#include "lili2d/ecs/ecs_view.hpp"
#include "lili2d/ecs/entity.hpp"

namespace systems {

void
updateMovement(
    lili::ECSRegistry& registry, float dt, float window_w, float window_h
) {
    auto view =
        registry.view<PositionComponent, VelocityComponent, RenderComponent>();
    for (auto [entity, pos, vel, render] : view) {
        pos.value += vel.value * dt;

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

void
renderEntities(lili::ECSRegistry& registry, lili::SpriteBatch& batch) {
    batch.begin();

    auto view = registry.view<PositionComponent, RenderComponent>();

    for (auto [entity, pos, render] : view)
        batch.draw(
            render.slice, pos.value,
            {render.radius * 2.0f, render.radius * 2.0f}, 0.0f, render.color
        );

    batch.end();
    batch.draw();
}

}  // namespace systems
