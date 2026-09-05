#include "lili2d/render/scene/core2d/animated_sprite.hpp"

#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/utils.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/renderer.hpp"
#include "lili2d/render/scene/common/animation_registry.hpp"

namespace lili {

AnimatedSprite::AnimatedSprite(Renderer* renderer, const Animation& animation)
    : renderer(renderer), animation(animation) {
    mesh = renderer->getUnitQuad();
    material = std::make_unique<Material>(renderer->getTheWhitePixel());
    material->properties.color_tint = {1.0f, 1.0f, 1.0f, 1.0f};

    position = {0.0f, 0.0f};
    scale = {1.0f, 1.0f};
    rotation = 0.0f;
    layer = 0.0f;
    current_frame = 0;
    frame_time_sec = 0.0f;

    if (animation.frameCount() > 0) applyFrame(animation.getFrame(0));
}

AnimatedSprite::AnimatedSprite(
    Renderer* renderer, std::string_view animation_key
)
    : AnimatedSprite(
          renderer, AnimationRegistry::get().getAnimation(animation_key)
      ) {}

AnimatedSprite::AnimatedSprite(Renderer* renderer, uint16_t animation_id)
    : AnimatedSprite(
          renderer, AnimationRegistry::get().getAnimation(animation_id)
      ) {}

void
AnimatedSprite::setAnimation(const Animation& animation) {
    this->animation = animation;
    current_frame = 0;
    frame_time_sec = 0.0f;

    if (animation.frameCount() > 0) applyFrame(animation.getFrame(0));
}

void
AnimatedSprite::setAnimation(std::string_view animation_key) {
    setAnimation(AnimationRegistry::get().getAnimation(animation_key));
}

void
AnimatedSprite::setAnimation(uint16_t animation_id) {
    setAnimation(AnimationRegistry::get().getAnimation(animation_id));
}

Mat3
AnimatedSprite::getTransformMatrix() const {
    if (render_layer == RenderLayer::UI && renderer) {
        Vec2 viewport_size = renderer->getLogicalResolution();
        Vec2 obj_size = getSize();
        return ui_layout.getTransformationMatrix(
            viewport_size, obj_size, rotation, obj_size
        );
    }
    return Mat3::translate(position) * Mat3::rotation(rotation) *
           Mat3::scale(getSize());
}

void
AnimatedSprite::update(float dt) {
    if (animation.frameCount() <= 1) return;

    frame_time_sec += dt;
    if (frame_time_sec >= frame_speed_sec) {
        frame_time_sec = 0.0f;
        current_frame = (current_frame + 1) % animation.frameCount();
        applyFrame(animation.getFrame(current_frame));
    }
}

void
AnimatedSprite::reset() noexcept {
    current_frame = 0;
    frame_time_sec = 0.0f;
    if (animation.frameCount() > 0) applyFrame(animation.getFrame(0));
}

void
AnimatedSprite::draw() {
    if (!is_visible) return;
    Mat3 mat_transform = getTransformMatrix();
    renderer->submit(
        Model(mesh, getMaterial()), mat_transform, layer, render_layer
    );
}

void
AnimatedSprite::applyFrame(const SliceUV& frame) {
    Material* mat = getMaterial();
    if (mat) {
        mat->albedoMap = frame.texture;
        mat->properties.uv_bounds =
            Vec4(frame.u_min, frame.v_min, frame.u_max, frame.v_max);
    }
    this->size = Vec2(frame.width, frame.height);
}

}  // namespace lili
