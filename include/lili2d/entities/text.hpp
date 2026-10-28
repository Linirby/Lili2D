#pragma once

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/2d/bitmap_font.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include <string_view>
#include <type_traits>

namespace lili {

struct TextParameters
{
    std::string_view text = "";
    Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    Vec2 pos = { 0.0f, 0.0f };
    Vec2 scale = { 1.0f, 1.0f };
    float rotation = 0.0f;
    float layer = 0.0f;
    float spacing = 1.0f;
    uint16_t material_id = 0;
    bool is_visible = true;
    RenderLayer render_pass = RenderLayer::WORLD2D;
};

static_assert(
    std::is_trivially_copyable_v<TextParameters>,
    "TextParameters must be trivially copyable."
);

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
createText(
    ECSRegistry& registry,
    BitmapFont* font,
    const TextParameters& params
);

} // namespace lili
