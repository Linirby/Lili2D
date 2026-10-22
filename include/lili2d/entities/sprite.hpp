#pragma once

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/common/atlas_map.hpp"
#include "lili2d/render/gpu/pass_types.hpp"
#include <cstdint>
#include <string>

namespace lili {

class Renderer;

struct SpriteParameters
{
    Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    Vec2 pos = { 0.0f, 0.0f };
    Vec2 scale = { 1.0f, 1.0f };
    float rotation = 0.0f;
    float layer = 0.0f;
    uint16_t material_id = 0;
    bool is_visible = true;
    RenderLayer render_pass = RenderLayer::WORLD2D;
};

static_assert(
    std::is_trivially_copyable_v<SpriteParameters>,
    "SpriteParameters must be tryvially copyable."
);

/// @brief Creates a 2D Sprite entity from an image file path.
/// @note Attached components:
/// - `TransformComponent` (`pos`, `prev_pos`, `scale`, `rotation`)
/// - `Render2DComponent` (`color`, `size`, `layer`, `material_id`,
/// `render_pass`, `is_visible`)
/// - `SpriteComponent` (`slice`)
/// @param registry The ECS registry.
/// @param renderer Pointer to the Renderer.
/// @param img_path Path to the texture image file.
/// @param params The parameters of the sprite.
/// @return The created Entity.
[[nodiscard]] Entity
createSprite(
    ECSRegistry& registry,
    Renderer* renderer,
    const std::string& img_path,
    SpriteParameters params
);

/// @brief Creates a 2D Sprite entity from an existing UV slice.
/// @note Attached components:
/// - `TransformComponent` (`pos`, `prev_pos`, `scale`, `rotation`)
/// - `Render2DComponent` (`color`, `size`, `layer`, `material_id`,
/// `render_pass`, `is_visible`)
/// - `SpriteComponent` (`slice`)
/// @param registry The ECS registry.
/// @param sliceUV The UV slice and texture pointer.
/// @param params The parameters of the sprite.
/// @return The created Entity.
[[nodiscard]] Entity
createSprite(
    ECSRegistry& registry,
    const SliceUV& sliceUV,
    SpriteParameters params
);

} // namespace lili
