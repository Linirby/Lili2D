#pragma once

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/gpu/pass_types.hpp"

namespace lili {

/// @brief Creates a 2D Line (Geometry) entity.
/// @note Attached components:
/// - `TransformComponent` (`pos`, `prev_pos`, `scale`, `rotation`)
/// - `Render2DComponent` (`color`, `size`, `layer`, `material_id`, `render_pass`, `is_visible`)
/// - `LineComponent` (`dxy`, `thickness`)
/// @param registry The ECS registry.
/// @param start_pos Starting point in pixels (stored in `TransformComponent::pos`).
/// @param end_pos Ending point in pixels (`end_pos - start_pos` stored in `LineComponent::dxy`).
/// @param thickness Line thickness in pixels.
/// @param color RGBA color.
/// @param layer Z-ordering depth layer.
/// @param render_pass Target render pass (`WORLD2D` or `UI`).
/// @param is_visible Initial visibility flag.
/// @param material_id Linked material ID.
/// @return The created Entity.
[[nodiscard]] Entity
createLine(
    ECSRegistry& registry,
    Vec2 start_pos,
    Vec2 end_pos,
    float thickness = 1.0f,
    Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f },
    float layer = 0.0f,
    RenderLayer render_pass = RenderLayer::WORLD2D,
    bool is_visible = true,
    uint16_t material_id = 0
);

} // namespace lili
