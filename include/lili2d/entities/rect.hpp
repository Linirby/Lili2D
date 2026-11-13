#pragma once

#include "lili2d/components/rect.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/ecs_registry.hpp"

namespace lili {

/// @brief Parameters used to initialize a rectangle entity.
struct RectParameters
{
    cTransform transform{}; ///< Transform component.
    cRender2D render2d{};   ///< Render2D component.
    cRect rect{};           ///< Rect component.
};

/// @brief Creates a 2D Rectangle (Quad) entity.
/// @note Attached components:
/// - `cTransform` (`pos`, `scale`, `rotation`)
/// - `cRender2D` (`color`, `size`, `layer`, `material_id`,
/// `render_pass`, `is_visible`)
/// - `cRect` (`thickness`, `hollow`)
/// @param registry The ECS registry.
/// @param params The parameters of the rect.
/// @return The created Entity.
[[nodiscard]] Entity
createRect(ECSRegistry& registry, const RectParameters& params = {});

} // namespace lili
