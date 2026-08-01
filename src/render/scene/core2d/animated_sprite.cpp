#include "lili2d/render/scene/core2d/animated_sprite.hpp"

#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/utils.hpp"
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
    Renderer* renderer, const std::string& animation_key
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
AnimatedSprite::setAnimation(const std::string& animation_key) {
    setAnimation(AnimationRegistry::get().getAnimation(animation_key));
}

void
AnimatedSprite::setAnimation(uint16_t animation_id) {
    setAnimation(AnimationRegistry::get().getAnimation(animation_id));
}

void
AnimatedSprite::setFrameSpeed(float speed_sec) {
    frame_speed_sec = speed_sec;
}

void
AnimatedSprite::setColorTint(const Vec4& color) {
    if (material) {
        material->properties.color_tint = color;
    }
}

void
AnimatedSprite::setColor(Vec4 color) {
    setColorTint(color);
}

void
AnimatedSprite::setMaterial(Material* material) {
    external_material = material;
}

void
AnimatedSprite::setPosition(Vec2 position) {
    this->position = position;
    ui_layout.offset = position;
}


void
AnimatedSprite::setScale(Vec2 scale) {
    this->scale = scale;
}

void
AnimatedSprite::setSize(Vec2 size) {
    this->size = size;
}

void
AnimatedSprite::setRotation(float degree) {
    rotation = lili::degToRad(degree);
}

void
AnimatedSprite::setLayer(float layer) {
    this->layer = layer;
}

void
AnimatedSprite::setRender(RenderLayer render_layer) {
    this->render_layer = render_layer;
}

Vec2
AnimatedSprite::getPosition() const {
    return position;
}

float
AnimatedSprite::getRotation() const {
    return lili::radToDeg(rotation);
}

Vec2
AnimatedSprite::getScale() const {
    return scale;
}

float
AnimatedSprite::getWidth() const {
    return size.x * scale.x;
}

float
AnimatedSprite::getHeight() const {
    return size.y * scale.y;
}

Vec2
AnimatedSprite::getSize() const {
    return Vec2(size.x * scale.x, size.y * scale.y);
}

Mat3
AnimatedSprite::getTransformMatrix() const {
    if (render_layer == RenderLayer::UI && renderer) {
        Vec2 viewport_size = {
            static_cast<float>(renderer->getSwapchainWidth()),
            static_cast<float>(renderer->getSwapchainHeight())
        };
        Vec2 obj_size = getSize();
        return ui_layout.getTransformationMatrix(
            viewport_size, obj_size, rotation, obj_size
        );
    }
    return Mat3::translate(position) * Mat3::rotation(rotation) *
           Mat3::scale(getSize());
}


float
AnimatedSprite::getLayer() const {
    return layer;
}

RenderLayer
AnimatedSprite::getRender() const {
    return render_layer;
}

Vec4
AnimatedSprite::getColor() const {
    Material* mat = getMaterial();
    return mat ? mat->properties.color_tint : Vec4(1, 1, 1, 1);
}

Material*
AnimatedSprite::getMaterial() const {
    return external_material ? external_material : material.get();
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
AnimatedSprite::reset() {
    current_frame = 0;
    frame_time_sec = 0.0f;
    if (animation.frameCount() > 0) applyFrame(animation.getFrame(0));
}

void
AnimatedSprite::draw() {
    if (!is_visible) return;
    Mat3 mat_transform = getTransformMatrix();
    renderer->submit(
        Model({mesh, getMaterial()}), mat_transform, layer, render_layer
    );
}

void
AnimatedSprite::applyFrame(const SliceUV& frame) {
    Material* mat = getMaterial();
    if (mat) {
        mat->albedoMap = frame.texture;
        mat->properties.uv_bounds = {
            frame.u_min, frame.v_min, frame.u_max, frame.v_max
        };
    }
    this->size = {frame.width, frame.height};
}

}  // namespace lili
