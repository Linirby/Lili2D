#include "lili2d/render/scene/common/animation_player.hpp"

#include "lili2d/render/scene/common/animation_registry.hpp"

namespace lili {

AnimationPlayer::AnimationPlayer(uint16_t animation_id) {
    setAnimation(animation_id);
}

AnimationPlayer::AnimationPlayer(std::string_view animation_key) {
    setAnimation(animation_key);
}

void
AnimationPlayer::setAnimation(uint16_t animation_id) {
    this->animation = &AnimationRegistry::get().getAnimation(animation_id);
    reset();
}

void
AnimationPlayer::setAnimation(std::string_view animation_key) {
    this->animation = &AnimationRegistry::get().getAnimation(animation_key);
    reset();
}

void
AnimationPlayer::update(float dt) noexcept {
    if (!animation || animation->frameCount() == 0) return;

    frame_time_sec += dt;
    if (frame_time_sec >= frame_speed_sec) {
        frame_time_sec -= frame_speed_sec;
        current_frame++;
        if (current_frame >= animation->frameCount()) current_frame = 0;
    }
}

const SliceUV&
AnimationPlayer::getCurrentFrame() const noexcept {
    if (!animation || animation->frameCount() == 0) {
        static SliceUV empty_slice;
        return empty_slice;
    }
    return animation->getFrame(current_frame);
}

}  // namespace lili
