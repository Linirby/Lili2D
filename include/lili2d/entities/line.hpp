#pragma once

#include "lili2d/components/line.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/ecs_registry.hpp"

namespace lili {

/// @brief Parameters used to initialize a line entity.
struct LineParameters
{
    TransformComponent transform{}; ///< Transform component.
    Render2DComponent render2d{};   ///< Render2D component.
    LineComponent line{};           ///< Line component.
};

/// @brief Creates a 2D Line (Geometry) entity.
/// @note Attached components:
/// - `TransformComponent` (`pos`, `scale`, `rotation`)
/// - `Render2DComponent` (`color`, `layer`, `material_id`,
/// `render_pass`, `is_visible`)
/// - `LineComponent` (`dxy`, `thickness`)
/// @param registry The ECS registry.
/// @param params The parameters of the line.
/// @return The created Entity.
[[nodiscard]] Entity
createLine(ECSRegistry& registry, const LineParameters& params = {});

} // namespace lili
