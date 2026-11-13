#pragma once

#include "lili2d/components/line.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/ecs_registry.hpp"

namespace lili {

/// @brief Parameters used to initialize a line entity.
struct LineParameters
{
    cTransform transform{}; ///< Transform component.
    cRender2D render2d{};   ///< Render2D component.
    cLine line{};           ///< Line component.
};

/// @brief Creates a 2D Line (Geometry) entity.
/// @note Attached components:
/// - `cTransform` (`pos`, `scale`, `rotation`)
/// - `cRender2D` (`color`, `layer`, `material_id`,
/// `render_pass`, `is_visible`)
/// - `cLine` (`dxy`, `thickness`)
/// @param registry The ECS registry.
/// @param params The parameters of the line.
/// @return The created Entity.
[[nodiscard]] Entity
createLine(ECSRegistry& registry, const LineParameters& params = {});

} // namespace lili
