#pragma once

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/gpu/pass_types.hpp"

namespace lili {

[[nodiscard]] Entity
createRect(
    ECSRegistry& registry,
    Vec2 pos,
    Vec2 size,
    Vec2 scale = { 1.0f, 1.0f },
    float rotation = 0.0f,
    Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f },
    bool hollow = false,
    float thickness = 0.0f,
    float layer = 0.0f,
    RenderLayer render_pass = RenderLayer::WORLD2D,
    bool is_visible = true,
    uint16_t material_id = 0
);

} // namespace lili
