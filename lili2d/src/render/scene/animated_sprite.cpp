#include "render/scene/animated_sprite.hpp"

#include "render/scene/utils.hpp"
#include "geometry/mat3x3.hpp"
#include "geometry/utils.hpp"

namespace lili {

AnimatedSprite::AnimatedSprite(
	Renderer *renderer, const Animation &animation
) {
	this->renderer = renderer;
	this->animation = animation;

	MeshData mesh_data = create_unit_quad();
	mesh = std::make_unique<GPUMesh>(renderer->get_device(), mesh_data);
	material = std::make_unique<Material>(renderer->get_the_white_pixel());
	material->properties.color_tint = { 1.0f, 1.0f, 1.0f, 1.0f };

	position = { 0.0f, 0.0f };
	scale = { 1.0f, 1.0f };
	rotation = 0.0f;
	layer = 0.0f;
	current_frame = 0;
	frame_time_sec = 0.0f;

	if (animation.frame_count() > 0)
		apply_frame(animation.get_frame(0));
}

AnimatedSprite::AnimatedSprite(
	Renderer *renderer, const std::string &animation_key
) : AnimatedSprite(
	renderer, AnimationRegistry::get().get_animation(animation_key)
) {}

AnimatedSprite::AnimatedSprite(
	Renderer *renderer, uint16_t animation_id
) : AnimatedSprite(
	renderer, AnimationRegistry::get().get_animation(animation_id)
) {}

void AnimatedSprite::set_animation(const Animation &animation) {
	this->animation = animation;
	current_frame = 0;
	frame_time_sec = 0.0f;

	if (animation.frame_count() > 0)
		apply_frame(animation.get_frame(0));
}

void AnimatedSprite::set_animation(const std::string &animation_key) {
	set_animation(AnimationRegistry::get().get_animation(animation_key));
}

void AnimatedSprite::set_animation(uint16_t animation_id) {
	set_animation(AnimationRegistry::get().get_animation(animation_id));
}

void AnimatedSprite::set_frame_speed(float speed_sec) {
	frame_speed_sec = speed_sec;
}

void AnimatedSprite::set_color_tint(const Vec4 &color) {
	material->properties.color_tint = color;
}

void AnimatedSprite::set_position(const Vec2 &position) {
	this->position = position;
}

void AnimatedSprite::set_scale(const Vec2 &scale) {
	this->scale = scale;
}

void AnimatedSprite::set_size(const Vec2 &size) {
	this->size = size;
}

void AnimatedSprite::set_rotation(float degree) {
	rotation = lili::deg_to_rad(degree);
}

void AnimatedSprite::set_layer(float layer) {
	this->layer = layer;
}

float AnimatedSprite::get_width() const {
	return size.x * scale.x;
}

float AnimatedSprite::get_height() const {
	return size.y * scale.y;
}

Vec2 AnimatedSprite::get_size() const {
	return Vec2(size.x * scale.x, size.y * scale.y);
}

Material* AnimatedSprite::get_material() const {
	return material.get();
}

void AnimatedSprite::update(float dt) {
	if (animation.frame_count() <= 1) return;

	frame_time_sec += dt;
	if (frame_time_sec >= frame_speed_sec) {
		frame_time_sec = 0.0f;
		current_frame = (current_frame + 1) % animation.frame_count();
		apply_frame(animation.get_frame(current_frame));
	}
}

void AnimatedSprite::reset() {
	current_frame = 0;
	frame_time_sec = 0.0f;
	if (animation.frame_count() > 0)
		apply_frame(animation.get_frame(0));
}

void AnimatedSprite::draw() {
	Mat3 mat_transform = (
		Mat3::translate(position) *
		Mat3::rotation(rotation) *
		Mat3::scale(get_size())
	);
	renderer->submit(
		(Model){ mesh.get(), material.get() },
		mat_transform,
		layer,
		RenderLayer::WORLD2D
	);
}

void AnimatedSprite::apply_frame(const AnimationFrame &frame) {
	material->albedo_map = frame.texture;

	MeshData mesh_data = create_unit_quad();
	mesh_data.vertices[0].u = frame.u_min;
	mesh_data.vertices[0].v = frame.v_min;
	mesh_data.vertices[1].u = frame.u_max;
	mesh_data.vertices[1].v = frame.v_min;
	mesh_data.vertices[2].u = frame.u_max;
	mesh_data.vertices[2].v = frame.v_max;
	mesh_data.vertices[3].u = frame.u_min;
	mesh_data.vertices[3].v = frame.v_max;

	mesh = std::make_unique<GPUMesh>(renderer->get_device(), mesh_data);
	this->size = { frame.width, frame.height };
}

}  // namespace lili
