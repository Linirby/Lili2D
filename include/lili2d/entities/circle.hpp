#pragma once

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include <cstdint>

namespace lili {

struct CircleParameters
{
    Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    Vec2 pos = { 0.0f, 0.0f };
    float radius = 10.0f;
    float thickness = 1.0f;
    float layer = 0.0f;
    uint32_t segments = 16;
    uint16_t material_id = 0;
    bool hollow = false;
    bool is_visible = true;
    RenderLayer render_pass = RenderLayer::WORLD2D;
};

static_assert(
    std::is_trivially_copyable_v<CircleParameters>,
    "CircleParameters must be tryvially copyable."
);

/// @brief Creates a 2D Circle (Geometry) entity.
/// Attached components:
/// - `TransformComponent` (`pos`, `prev_pos`, `scale`, `rotation`)
/// - `Render2DComponent` (`color`, `size`, `layer`, `material_id`,
/// `render_pass`, `is_visible`)
/// - `CircleComponent` (`radius`, `segments`, `thickness`, `hollow`)
/// @param registry The ECS registry.
/// @param params The parameters for the circle.
/// @return The created Entity.
[[nodiscard]] Entity
createCircle(ECSRegistry& registry, const CircleParameters& params);

} // namespace lili
