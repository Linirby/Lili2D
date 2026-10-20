#pragma once

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/gpu/pass_types.hpp"

namespace lili {

/// @brief Creates a 2D Circle (Geometry) entity.
/// @note Attached components:
/// - `TransformComponent` (`pos`, `prev_pos`, `scale`, `rotation`)
/// - `Render2DComponent` (`color`, `size`, `layer`, `material_id`, `render_pass`, `is_visible`)
/// - `CircleComponent` (`radius`, `segments`, `thickness`, `hollow`)
/// @param registry The ECS registry.
/// @param pos Center position in pixels.
/// @param radius Circle radius in pixels (stored in `CircleComponent::radius`).
/// @param segments Number of polygon segments.
/// @param color RGBA color.
/// @param hollow True for outline only, false for filled.
/// @param thickness Outline thickness when hollow.
/// @param layer Z-ordering depth layer.
/// @param render_pass Target render pass (`WORLD2D` or `UI`).
/// @param is_visible Initial visibility flag.
/// @param material_id Linked material ID.
/// @return The created Entity.
[[nodiscard]] Entity
createCircle(
    ECSRegistry& registry,
    Vec2 pos,
    float radius,
    uint32_t segments = 24,
    Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f },
    bool hollow = false,
    float thickness = 1.0f,
    float layer = 0.0f,
    RenderLayer render_pass = RenderLayer::WORLD2D,
    bool is_visible = true,
    uint16_t material_id = 0
);

} // namespace lili
