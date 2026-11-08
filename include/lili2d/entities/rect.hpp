#pragma once

#include "lili2d/components/rect.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/ecs_registry.hpp"

namespace lili {

struct RectParameters
{
    TransformComponent transform{};
    Render2DComponent render2d{};
    RectComponent rect{};
};

/// @brief Creates a 2D Rectangle (Quad) entity.
/// @note Attached components:
/// - `TransformComponent` (`pos`, `scale`, `rotation`)
/// - `Render2DComponent` (`color`, `size`, `layer`, `material_id`,
/// `render_pass`, `is_visible`)
/// - `RectComponent` (`thickness`, `hollow`)
/// @param registry The ECS registry.
/// @param params The parameters of the rect.
/// @return The created Entity.
[[nodiscard]] Entity
createRect(ECSRegistry& registry, const RectParameters& params = {});

} // namespace lili
