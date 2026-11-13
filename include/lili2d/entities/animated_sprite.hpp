#pragma once

#include "lili2d/components/animation.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/sprite.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/ecs_registry.hpp"

namespace lili {

/// @brief Parameters used to initialize an animated sprite entity.
struct AnimatedSpriteParameters
{
    cTransform transform{}; ///< Transform component.
    cRender2D render2d{};   ///< Render2D component.
    cSprite sprite{};       ///< Sprite component.
    cAnimation animation{}; ///< Animation component.
};

/// @brief Creates a 2D Sprite entity with an Animation.
/// @note Attached components:
/// - `cTransform` (`pos`, `scale`, `rotation`)
/// - `cRender2D` (`color`, `layer`, `material_id`,
/// `render_pass`, `is_visible`)
/// - `cSprite` (managed automatically)
/// - `AnimationComponent` (`frame_time`, `current_frame`)
/// @param registry The ECS registry.
/// @param anim_key Key to the registered Animation.
/// @param params The parameters of the sprite.
/// @return The created Entity.
[[nodiscard]] Entity
createAnimatedSprite(
    ECSRegistry& registry,
    std::string_view anim_key,
    const AnimatedSpriteParameters& params = {}
);

} // namespace lili
