#pragma once

#include "lili2d/components/circle.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/ecs_registry.hpp"

namespace lili {

struct CircleParameters
{
    TransformComponent transform{};
    Render2DComponent render2d{};
    CircleComponent circle{};
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
