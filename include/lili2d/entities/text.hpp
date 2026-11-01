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
    std::string_view content = "";
    TransformComponent transform{};
    Render2DComponent render2d{};
    TextComponent text{};
};

/// @brief Creates a 2D Text entity using a BitmapFont and StringArena.
/// @note Attached components:
/// - `TransformComponent` (`pos`, `prev_pos`, `scale`, `rotation`)
/// - `Render2DComponent` (`color`, `size`, `layer`, `material_id`,
/// `render_pass`, `is_visible`)
/// - `TextComponent` (`font`, `glyph_size`, `spacing`, `offset`, `length`)
/// @param registry The ECS registry.
/// @param params The parameters of the text entity.
/// @return The created Entity.
[[nodiscard]] Entity
createText(ECSRegistry& registry, const TextParameters& params);

} // namespace lili
