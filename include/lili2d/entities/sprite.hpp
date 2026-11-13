#pragma once

#include "lili2d/components/render2d.hpp"
#include "lili2d/components/sprite.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/render/common/slice_uv.hpp"
#include <string>

namespace lili {

class Renderer;

/// @brief Parameters used to initialize a sprite entity.
struct SpriteParameters
{
    cTransform transform{}; ///< Transform component.
    cRender2D render2d{};   ///< Render2D component.
    cSprite sprite{};       ///< Sprite component.
};

static_assert(
    std::is_trivially_copyable_v<SpriteParameters>,
    "SpriteParameters must be tryvially copyable."
);

/// @brief Creates a 2D Sprite entity from an image file path.
/// @note Attached components:
/// - `cTransform` (`pos`, `scale`, `rotation`)
/// - `cRender2D` (`color`, `layer`, `material_id`,
/// `render_pass`, `is_visible`)
/// - `cSprite` (managed automatically)
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
    SpriteParameters params = {}
);

/// @brief Creates a 2D Sprite entity from an existing UV slice.
/// @note Attached components:
/// - `cTransform` (`pos`, `scale`, `rotation`)
/// - `cRender2D` (`color`, `size`, `layer`, `material_id`,
/// `render_pass`, `is_visible`)
/// - `cSprite` (managed automatically)
/// @param registry The ECS registry.
/// @param sliceUV The UV slice and texture pointer.
/// @param params The parameters of the sprite.
/// @return The created Entity.
[[nodiscard]] Entity
createSprite(
    ECSRegistry& registry,
    const SliceUV& sliceUV,
    SpriteParameters params = {}
);

} // namespace lili
