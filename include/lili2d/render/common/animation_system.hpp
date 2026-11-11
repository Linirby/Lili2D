#pragma once

#include "lili2d/components/animation.hpp"
#include "lili2d/components/sprite.hpp"
#include "lili2d/ecs/system.hpp"

namespace lili {

/// @brief ECS system that advances sprite animations and updates UV coordinates
/// on SpriteComponent.
class AnimationSystem : public System<AnimationComponent, SpriteComponent>
{
public:
    /// @brief Default constructor.
    explicit AnimationSystem() noexcept {}

    /// @brief Advances the animation timer and updates the sprite UV slice for
    /// a single entity.
    /// @param registry The ECS registry.
    /// @param entity The entity being updated.
    /// @param anim_comp The entity's AnimationComponent.
    /// @param sprite_comp The entity's SpriteComponent.
    /// @param dt The frame delta time in seconds.
    void
    updateEntity(
        ECSRegistry& registry,
        Entity entity,
        lili::AnimationComponent& anim_comp,
        lili::SpriteComponent& sprite_comp,
        float dt
    ) override;

    /// @brief Updates all entities that have both AnimationComponent and
    /// SpriteComponent.
    /// @param registry The ECS registry.
    /// @param dt The frame delta time in seconds.
    /// @param thread_pool Optional thread pool for parallel execution.
    static void
    update(ECSRegistry& registry, float dt, ThreadPool* thread_pool = nullptr);
};

} // namespace lili
