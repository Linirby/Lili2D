#include "lili2d/render/common/animation_system.hpp"

namespace lili {

void
AnimationSystem::run(ECSRegistry& registry, float dt, ThreadPool* thread_pool)
{
}

void
AnimationSystem::updateEntity(
    ECSRegistry& registry,
    Entity entity,
    lili::AnimationComponent& anim_comp,
    lili::SpriteComponent& sprite_comp,
    float dt
)
{
}

} // namespace lili
