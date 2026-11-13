#include "lili2d/entities/animated_sprite.hpp"
#include "lili2d/components/animation.hpp"
#include "lili2d/components/render2d.hpp"
#include "lili2d/components/sprite.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/ecs/entity.hpp"
#include "lili2d/render/common/animation_registry.hpp"
#include "lili2d/render/common/slice_uv.hpp"
#include <cstdint>
#include <stdexcept>

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

    const Animation& anim = anim_reg.getAnimation(anim_key);
    if (anim.frameCount() == 0)
        throw std::runtime_error(
            "You can't create an AnimatedSprite if the animation has 0 frame"
        );

    uint32_t frame = params.animation.current_frame < anim.frameCount()
                         ? params.animation.current_frame
                         : 0;
    SliceUV slice = anim.getFrame(frame);

    cTransform transform = params.transform;
    transform.prev_pos = params.transform.pos;

    cRender2D render2d = params.render2d;
    if (render2d.size == Vec2(0.0f, 0.0f))
        render2d.size = { slice.width, slice.height };

    cSprite sprite = params.sprite;
    sprite.slice = slice;

    cAnimation animation = params.animation;
    animation.current_frame = frame;
    animation.animation_id = anim_reg.getAnimationId(anim_key);
    animation.frame_timer = 0.0f;

    registry.addComponents(entity, transform, render2d, sprite, animation);
    return entity;
}

} // namespace lili
