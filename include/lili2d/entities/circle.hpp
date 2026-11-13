#pragma once

#include "lili2d/components/circle.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/ecs_registry.hpp"

namespace lili {

/// @brief Parameters to create a Circle
struct CircleParameters
{
    cTransform transform{}; ///< Transform component
    cRender2D render2d{};   ///< Render2D component
    cCircle circle{};       ///< Circle component
};

/// @brief Creates a 2D Circle (Geometry) entity.
/// @note Attached components:
/// - `cTransform` (`pos`, `scale`, `rotation`)
/// - `cRender2D` (`color`, `layer`, `material_id`,
/// `render_pass`, `is_visible`)
/// - `cCircle` (`radius`, `segments`, `thickness`, `hollow`)
/// @param registry The ECS registry.
/// @param params The parameters for the circle.
/// @return The created Entity.
[[nodiscard]] Entity
createCircle(ECSRegistry& registry, const CircleParameters& params = {});

} // namespace lili
