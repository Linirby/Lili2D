#pragma once

#include <lili2d/ecs/ecs_registry.hpp>
#include <lili2d/ecs/entity.hpp>
#include <lili2d/geometry/vec2.hpp>
#include <lili2d/geometry/vec4.hpp>
#include <lili2d/render/common/atlas_map.hpp>
#include <lili2d/render/renderer.hpp>

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
