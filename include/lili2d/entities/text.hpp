#pragma once

#include "lili2d/components/render2d.hpp"
#include "lili2d/components/text.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/entity.hpp"
#include <string_view>

namespace lili {

/// @brief Parameters used to initialize a text entity.
struct TextParameters
{
    cTransform transform{}; ///< Transform component.
    cRender2D render2d{};   ///< Render2D component.
    cText text{};           ///< Text component.
};

/// @brief Creates a 2D Text entity using a BitmapFont and StringArena.
/// @note Attached components:
/// - `cTransform` (`pos`, `scale`, `rotation`)
/// - `cRender2D` (`color`, `layer`, `material_id`,
/// `render_pass`, `is_visible`)
/// - `cText` (`font`, `spacing`)
/// @param registry The ECS registry.
/// @param content Initial text string content to store in the StringArena.
/// @param params The parameters of the text entity.
/// @return The created Entity.
[[nodiscard]] Entity
createText(
    ECSRegistry& registry,
    std::string_view content,
    const TextParameters& params = {}
);

} // namespace lili
