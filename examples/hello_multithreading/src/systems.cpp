#include "systems.hpp"
#include <lili2d/components/render2d.hpp>
#include <lili2d/components/transform.hpp>
#include <lili2d/geometry/vec2.hpp>

using namespace lili;

MovementSystem::MovementSystem(Vec2 window_size)
  : window_w(window_size.x)
  , window_h(window_size.y)
{
    parallel_threshold = 500;
}

void
MovementSystem::updateEntity(
    [[maybe_unused]] ECSRegistry& registry,
    [[maybe_unused]] Entity entity,
    TransformComponent& trans,
    VelocityComponent& vel,
    Render2DComponent& render2d,
    float dt
)
{
    trans.pos += vel.value * dt;

    if (trans.pos.x - render2d.size.x * 0.5f < 0.0f) {
        trans.pos.x = render2d.size.x * 0.5f;
        vel.value.x = -vel.value.x;
    } else if (trans.pos.x + render2d.size.x * 0.5f > window_w) {
        trans.pos.x = window_w - render2d.size.x * 0.5f;
        vel.value.x = -vel.value.x;
    }

    if (trans.pos.y - render2d.size.y * 0.5f < 0.0f) {
        trans.pos.y = render2d.size.y * 0.5f;
        vel.value.y = -vel.value.y;
    } else if (trans.pos.y + render2d.size.y * 0.5f > window_h) {
        trans.pos.y = window_h - render2d.size.y * 0.5f;
        vel.value.y = -vel.value.y;
    }
}
