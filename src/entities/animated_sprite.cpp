#include "lili2d/entities/animated_sprite.hpp"
#include "lili2d/components/animation.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/sprite.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/render/common/animation_registry.hpp"
#include "lili2d/render/common/slice_uv.hpp"

namespace lili {

Entity
createAnimatedSprite(
    ECSRegistry& registry,
    std::string_view anim_key,
    const AnimatedSpriteParameters& params
)
{
    AnimationRegistry& anim_reg = AnimationRegistry::get();

    Entity entity = registry.createEntity();

    SliceUV slice = anim_reg.getAnimation(anim_key).getFrame(
        params.animation.current_frame
    );

    TransformComponent transform = params.transform;
    transform.prev_pos = params.transform.pos;

    Render2DComponent render2d = params.render2d;
    render2d.size = { slice.width, slice.height };

    SpriteComponent sprite = params.sprite;
    sprite.slice = slice;

    AnimationComponent animation = params.animation;
    animation.animation_id = anim_reg.getAnimationId(anim_key);
    animation.frame_timer = 0.0f;

    registry.addComponents(entity, transform, render2d, sprite, animation);
    return entity;
}

} // namespace lili
