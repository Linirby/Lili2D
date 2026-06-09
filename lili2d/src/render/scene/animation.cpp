#include "render/scene/animation.hpp"
#include <stdexcept>

namespace lili {

Animation::Animation(const std::vector<AnimationFrame> &frames)
	: frames(frames) {}

size_t Animation::frame_count() const {
	return frames.size();
}

const AnimationFrame& Animation::get_frame(size_t index) const {
	return frames.at(index);
}

AnimationRegistry &AnimationRegistry::get() {
	static AnimationRegistry instance;
	return instance;
}

uint16_t AnimationRegistry::register_animation(
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

bool AnimationRegistry::has_animation(const std::string &key) const {
	return key_to_id.contains(key);
}

uint16_t AnimationRegistry::get_animation_id(const std::string &key) const {
	auto it = key_to_id.find(key);
	if (it == key_to_id.end())
		throw std::runtime_error("Animation key not found: " + key);
	return it->second;
}

const Animation &AnimationRegistry::get_animation(
	const std::string &key
) const {
	return get_animation(get_animation_id(key));
}

const Animation &AnimationRegistry::get_animation(uint16_t id) const {
	if (id >= id_to_animation.size())
		throw std::runtime_error("Animation ID out of range.");
	return id_to_animation[id];
}

const Animation &AnimationRegistry::get_animation(uint8_t id) const {
	return get_animation(static_cast<uint16_t>(id));
}

AnimationRegistry::AnimationRegistry() {
	register_animation("core:none", Animation());
}

size_t AnimationRegistry::animation_count() const {
	return id_to_animation.size();
}

const Animation *AnimationRegistry::animation_data() const {
	return id_to_animation.data();
}

}  // namespace lili
