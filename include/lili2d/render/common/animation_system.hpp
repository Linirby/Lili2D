#pragma once

#include "lili2d/components/animation.hpp"
#include "lili2d/components/sprite.hpp"
#include "lili2d/ecs/system.hpp"

namespace lili {

class AnimationSystem : public System<AnimationComponent, SpriteComponent>
{
public:
    explicit AnimationSystem() noexcept {}

    void
    updateEntity(
        ECSRegistry& registry,
        Entity entity,
        lili::AnimationComponent& anim_comp,
        lili::SpriteComponent& sprite_comp,
        float dt
    ) override;

    static void
    update(ECSRegistry& registry, float dt, ThreadPool* thread_pool = nullptr);
};

} // namespace lili
