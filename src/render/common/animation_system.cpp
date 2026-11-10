#include "lili2d/render/common/animation_system.hpp"
#include "lili2d/core/thread_pool.hpp"
#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/render/common/animation.hpp"
#include "lili2d/render/common/animation_registry.hpp"

namespace lili {

void
AnimationSystem::updateEntity(
    [[maybe_unused]] ECSRegistry& registry,
    [[maybe_unused]] Entity entity,
    lili::AnimationComponent& anim_comp,
    lili::SpriteComponent& sprite_comp,
    float dt
)
{
    AnimationRegistry& anim_reg = AnimationRegistry::get();

    if (anim_comp.animation_id >= anim_reg.animationCount())
        return;
    const Animation& anim = anim_reg.getAnimation(anim_comp.animation_id);

    if (anim.frameCount() == 0)
        return;

    anim_comp.frame_timer += dt;
    if (anim_comp.frame_timer >= anim_comp.frame_time) {
        anim_comp.frame_timer -= anim_comp.frame_time;
        anim_comp.current_frame++;
        if (anim_comp.current_frame >= anim.frameCount())
            anim_comp.current_frame = 0;
    }

    if (anim_comp.current_frame >= anim.frameCount())
        anim_comp.current_frame = 0;

    sprite_comp.slice = anim.getFrame(anim_comp.current_frame);
}

void
AnimationSystem::update(
    ECSRegistry& registry,
    float dt,
    ThreadPool* thread_pool
)
{
    auto sys = AnimationSystem();
    sys.run(registry, dt, thread_pool);
}

} // namespace lili
