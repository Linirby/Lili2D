#pragma once

#include "lili2d/components/animation.hpp"
#include "lili2d/components/sprite.hpp"
#include "lili2d/ecs/system.hpp"
namespace lili {

class AnimationSystem : public System<AnimationComponent, SpriteComponent>
{
public:
    void
    run(ECSRegistry& registry, float dt, ThreadPool* thread_pool) override;

private:
    void
    updateEntity(
        ECSRegistry& registry,
        Entity entity,
        lili::AnimationComponent& anim_comp,
        lili::SpriteComponent& sprite_comp,
        float dt
    ) override;
};

} // namespace lili
