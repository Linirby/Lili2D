#include "render/scene/core2d/animated_sprite.hpp"

#include "render/scene/common/utils.hpp"
#include "geometry/mat3x3.hpp"
#include "geometry/utils.hpp"

namespace lili {

AnimatedSprite::AnimatedSprite(Renderer *renderer, const Animation &animation)
	: renderer(renderer), animation(animation) {
	MeshData mesh_data = createUnitQuad();
	mesh = std::make_unique<GPUMesh>(renderer->getDevice(), mesh_data);
	material = std::make_unique<Material>(renderer->getTheWhitePixel());
	material->properties.color_tint = { 1.0f, 1.0f, 1.0f, 1.0f };

	position = { 0.0f, 0.0f };
	scale = { 1.0f, 1.0f };
	rotation = 0.0f;
	layer = 0.0f;
	current_frame = 0;
	frame_time_sec = 0.0f;

	if (animation.frameCount() > 0)
		applyFrame(animation.getFrame(0));
}

AnimatedSprite::AnimatedSprite(
	Renderer *renderer, const std::string &animation_key
) : AnimatedSprite(
	renderer, AnimationRegistry::get().getAnimation(animation_key)
) {}

AnimatedSprite::AnimatedSprite(Renderer *renderer, uint16_t animation_id)
	: AnimatedSprite(
	renderer, AnimationRegistry::get().getAnimation(animation_id)
) {}

void AnimatedSprite::setAnimation(const Animation &animation) {
	this->animation = animation;
	current_frame = 0;
	frame_time_sec = 0.0f;

	if (animation.frameCount() > 0)
		applyFrame(animation.getFrame(0));
}

void AnimatedSprite::setAnimation(const std::string &animation_key) {
	setAnimation(AnimationRegistry::get().getAnimation(animation_key));
}

void AnimatedSprite::setAnimation(uint16_t animation_id) {
	setAnimation(AnimationRegistry::get().getAnimation(animation_id));
}

void AnimatedSprite::setFrameSpeed(float speed_sec) {
	frame_speed_sec = speed_sec;
}

void AnimatedSprite::setColorTint(const Vec4 &color) {
	material->properties.color_tint = color;
}

void AnimatedSprite::setPosition(const Vec2 &position) {
	this->position = position;
}

void AnimatedSprite::setScale(const Vec2 &scale) {
	this->scale = scale;
}

void AnimatedSprite::setSize(const Vec2 &size) {
	this->size = size;
}

void AnimatedSprite::setRotation(float degree) {
	rotation = lili::degToRad(degree);
}

void AnimatedSprite::setLayer(float layer) {
	this->layer = layer;
}

float AnimatedSprite::getWidth() const {
	return size.x * scale.x;
}

float AnimatedSprite::getHeight() const {
	return size.y * scale.y;
}

Vec2 AnimatedSprite::getSize() const {
	return Vec2(size.x * scale.x, size.y * scale.y);
}

Material* AnimatedSprite::getMaterial() const {
	return material.get();
}

void AnimatedSprite::update(float dt) {
	if (animation.frameCount() <= 1) return;

	frame_time_sec += dt;
	if (frame_time_sec >= frame_speed_sec) {
		frame_time_sec = 0.0f;
		current_frame = (current_frame + 1) % animation.frameCount();
		applyFrame(animation.getFrame(current_frame));
	}
}

void AnimatedSprite::reset() {
	current_frame = 0;
	frame_time_sec = 0.0f;
	if (animation.frameCount() > 0)
		applyFrame(animation.getFrame(0));
}

void AnimatedSprite::draw() {
	Mat3 mat_transform = (
		Mat3::translate(position) *
		Mat3::rotation(rotation) *
		Mat3::scale(getSize())
	);
	renderer->submit(
		(Model){ mesh.get(), material.get() },
		mat_transform,
		layer,
		RenderLayer::WORLD2D
	);
}

void AnimatedSprite::applyFrame(const AnimationFrame &frame) {
	material->albedoMap = frame.texture;

	MeshData mesh_data = createUnitQuad();
	mesh_data.vertices[0].u = frame.u_min;
	mesh_data.vertices[0].v = frame.v_min;
	mesh_data.vertices[1].u = frame.u_max;
	mesh_data.vertices[1].v = frame.v_min;
	mesh_data.vertices[2].u = frame.u_max;
	mesh_data.vertices[2].v = frame.v_max;
	mesh_data.vertices[3].u = frame.u_min;
	mesh_data.vertices[3].v = frame.v_max;

	mesh = std::make_unique<GPUMesh>(renderer->getDevice(), mesh_data);
	this->size = { frame.width, frame.height };
}

}  // namespace lili
