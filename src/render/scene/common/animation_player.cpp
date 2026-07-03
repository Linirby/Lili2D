#include "lili2d/render/scene/common/animation_player.hpp"
#include "lili2d/render/scene/common/animation_registry.hpp"

namespace lili {

AnimationPlayer::AnimationPlayer(const Animation *animation)
	: animation(animation) {}

AnimationPlayer::AnimationPlayer(uint16_t animation_id) {
	setAnimation(animation_id);
}

AnimationPlayer::AnimationPlayer(const std::string &animation_key) {
	setAnimation(animation_key);
}

void AnimationPlayer::setAnimation(const Animation *animation) {
	this->animation = animation;
	reset();
}

void AnimationPlayer::setAnimation(uint16_t animation_id) {
	this->animation = &AnimationRegistry::get().getAnimation(animation_id);
	reset();
}

void AnimationPlayer::setAnimation(const std::string &animation_key) {
	this->animation = &AnimationRegistry::get().getAnimation(animation_key);
	reset();
}

void AnimationPlayer::setFrameSpeed(float speed_sec) {
	frame_speed_sec = speed_sec;
}

void AnimationPlayer::update(float dt) {
	if (!animation || animation->frameCount() == 0) return;

	frame_time_sec += dt;
	if (frame_time_sec >= frame_speed_sec) {
		frame_time_sec -= frame_speed_sec;
		current_frame++;
		if (current_frame >= animation->frameCount()) {
			current_frame = 0;
		}
	}
}

void AnimationPlayer::reset() {
	current_frame = 0;
	frame_time_sec = 0.0f;
}

const SliceUV& AnimationPlayer::getCurrentFrame() const {
	if (!animation || animation->frameCount() == 0) {
		static SliceUV empty_slice;
		return empty_slice;
	}
	return animation->getFrame(current_frame);
}

}  // namespace lili
