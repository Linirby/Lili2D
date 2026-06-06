#include "render/scene/animation.hpp"
#include "render/renderer.hpp"
#include <stdexcept>

namespace lili {

Animation::Animation(Renderer *renderer, const std::string &path) {
	texture = std::make_shared<Texture>(renderer->get_device(), path);
	build_single_frame();
}

Animation::Animation(const std::vector<AnimationFrame> &frames) {
	this->frames = frames;
}

void Animation::slice(int num_cols, int num_rows) {
	if (!texture)
		throw std::runtime_error(
			"Animation::slice: no texture loaded. "
			"Use the Animation(Renderer*, path) constructor first."
		);

	n_cols = num_cols;
	n_rows = num_rows;
	frames.clear();
	frames.reserve(num_cols * num_rows);

	float u_step = 1.0f / n_cols;
	float v_step = 1.0f / n_rows;
	float cell_w = static_cast<float>(texture->get_width()) / n_cols;
	float cell_h = static_cast<float>(texture->get_height()) / n_rows;

	for (int row = 0; row < num_rows; ++row) {
		for (int col = 0; col < num_cols; ++col) {
			AnimationFrame frame;
			frame.texture = texture.get();
			frame.u_min = col * u_step;
			frame.v_min = row * v_step;
			frame.u_max = (col + 1) * u_step;
			frame.v_max = (row + 1) * v_step;
			frame.width = cell_w;
			frame.height = cell_h;
			frames.push_back(frame);
		}
	}
}

size_t Animation::frame_count() const {
	return frames.size();
}

const AnimationFrame& Animation::get_frame(size_t index) const {
	return frames.at(index);
}

void Animation::build_single_frame() {
	frames.clear();
	AnimationFrame frame;
	frame.texture = texture.get();
	frame.u_min = 0.0f; frame.v_min = 0.0f;
	frame.u_max = 1.0f; frame.v_max = 1.0f;
	frame.width = static_cast<float>(texture->get_width());
	frame.height = static_cast<float>(texture->get_height());
	frames.push_back(frame);
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
