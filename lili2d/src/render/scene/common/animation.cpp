#include "render/scene/common/animation.hpp"
#include <stdexcept>

namespace lili {

Animation::Animation(const std::vector<AnimationFrame> &frames)
	: frames(frames) {}

size_t Animation::frameCount() const {
	return frames.size();
}

const AnimationFrame& Animation::getFrame(size_t index) const {
	return frames.at(index);
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

const Animation *AnimationRegistry::animation_data() const {
	return id_to_animation.data();
}

}  // namespace lili
