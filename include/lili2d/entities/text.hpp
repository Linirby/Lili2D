#pragma once

#include "lili2d/components/render2d.hpp"
#include "lili2d/components/text.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/entity.hpp"
#include <string_view>

namespace lili {

struct TextParameters
{
    TransformComponent transform{};
    Render2DComponent render2d{};
    TextComponent text{};
};

/// @brief Creates a 2D Text entity using a BitmapFont and StringArena.
/// @note Attached components:
/// - `TransformComponent` (`pos`, `scale`, `rotation`)
/// - `Render2DComponent` (`color`, `layer`, `material_id`,
/// `render_pass`, `is_visible`)
/// - `TextComponent` (`font`, `spacing`)
/// @param registry The ECS registry.
/// @param params The parameters of the text entity.
/// @return The created Entity.
[[nodiscard]] Entity
createText(
    ECSRegistry& registry,
    std::string_view content,
    const TextParameters& params = {}
);

} // namespace lili
