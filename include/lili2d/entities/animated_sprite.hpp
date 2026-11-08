#pragma once

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/entity.hpp"

namespace lili {

struct AnimatedSpriteParameters
{};

/// @brief Creates a 2D Sprite entity with an Animation.
/// @note Attached components:
/// - `TransformComponent` (`pos`, `scale`, `rotation`)
/// - `Render2DComponent` (`color`, `size`, `layer`, `material_id`,
/// `render_pass`, `is_visible`)
/// - `SpriteComponent` (managed automatically)
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
