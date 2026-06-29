#include "lili2d/render/scene/common/animation.hpp"
#include <stdexcept>

namespace lili {

Animation::Animation(const std::vector<SliceUV> &frames)
	: frames(frames) {}

size_t Animation::frameCount() const {
	return frames.size();
}

const SliceUV& Animation::getFrame(size_t index) const {
	return frames[index];
}

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

AnimationRegistry &AnimationRegistry::get() {
	static AnimationRegistry instance;
	return instance;
}

uint16_t AnimationRegistry::registerAnimation(
	const std::string &key, const Animation &animation
) {
	auto it = key_to_id.find(key);
	if (it != key_to_id.end()) {
		id_to_animation[it->second] = animation;
		return it->second;
	}

	if (id_to_animation.size() >= UINT16_MAX)
		throw std::runtime_error(
			"Animation registry reached uint16_t capacity."
		);

	uint16_t new_id = static_cast<uint16_t>(id_to_animation.size());
	key_to_id[key] = new_id;
	id_to_animation.push_back(animation);
	return new_id;
}

bool AnimationRegistry::hasAnimation(const std::string &key) const {
	return key_to_id.contains(key);
}

uint16_t AnimationRegistry::getAnimationId(const std::string &key) const {
	auto it = key_to_id.find(key);
	if (it == key_to_id.end())
		throw std::runtime_error("Animation key not found: " + key);
	return it->second;
}

const Animation &AnimationRegistry::getAnimation(
	const std::string &key
) const {
	return getAnimation(getAnimationId(key));
}

const Animation &AnimationRegistry::getAnimation(uint16_t id) const {
	if (id >= id_to_animation.size())
		throw std::runtime_error("Animation ID out of range.");
	return id_to_animation[id];
}

const Animation &AnimationRegistry::getAnimation(uint8_t id) const {
	return getAnimation(static_cast<uint16_t>(id));
}

AnimationRegistry::AnimationRegistry() {
	registerAnimation("core:none", Animation());
}

size_t AnimationRegistry::animationCount() const {
	return id_to_animation.size();
}

const Animation *AnimationRegistry::animationData() const {
	return id_to_animation.data();
}

}  // namespace lili
