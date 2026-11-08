#pragma once

#include <lili2d/ecs/ecs_registry.hpp>

namespace entities {

lili::Entity
spawnBall(
    lili::ECSRegistry& registry,
    lili::Renderer* renderer,
    lili::Vec2 pos,
    lili::Vec2 vel,
    lili::Vec4 color,
    float scale
);

} // namespace entities
