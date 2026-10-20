#pragma once

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include <cstdint>

namespace lili {

struct LineParameters
{
    Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    Vec2 start_pos = { 0.0f, 0.0f };
    Vec2 end_pos = { 10.0f, 0.0f };
    float thickness = 1.0f;
    float layer = 0.0f;
    uint16_t material_id = 0;
    bool is_visible = true;
    RenderLayer render_pass = RenderLayer::WORLD2D;
};

static_assert(
    std::is_trivially_copyable_v<LineParameters>,
    "LineParameters must be tryvially copyable."
);

/// @brief Creates a 2D Line (Geometry) entity.
/// @note Attached components:
/// - `TransformComponent` (`pos`, `prev_pos`, `scale`, `rotation`)
/// - `Render2DComponent` (`color`, `size`, `layer`, `material_id`,
/// `render_pass`, `is_visible`)
/// - `LineComponent` (`dxy`, `thickness`)
/// @param registry The ECS registry.
/// @param params The parameters of the line.
/// @return The created Entity.
[[nodiscard]] Entity
createLine(ECSRegistry& registry, const LineParameters& params);

} // namespace lili
