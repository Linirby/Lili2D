#include "lili2d/render/scene/core2d/sprite.hpp"

#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/utils.hpp"
#include "lili2d/render/scene/common/utils.hpp"

namespace lili {

Sprite::Sprite(Renderer* renderer, const std::string& path)
    : renderer(renderer) {
    texture = std::make_unique<Texture>(renderer->getDevice(), path);
    mesh = renderer->getUnitQuad();

    material = std::make_unique<Material>(texture.get());
    material->properties.color_tint = {1.0f, 1.0f, 1.0f, 1.0f};

    position = {0.0f, 0.0f};
    scale = {1.0f, 1.0f};
    size = {(float)texture->getWidth(), (float)texture->getHeight()};
    rotation = 0.0f;
    layer = 0.0f;
}

Sprite::Sprite(Renderer* renderer, SliceUV slice) : renderer(renderer) {
    mesh = renderer->getUnitQuad();

    material = std::make_unique<Material>(slice.texture);
    material->properties.color_tint = {1.0f, 1.0f, 1.0f, 1.0f};

    position = {0.0f, 0.0f};
    scale = {1.0f, 1.0f};
    size = {slice.width, slice.height};
    rotation = 0.0f;
    layer = 0.0f;
    setSlice(slice);
}

void
Sprite::setImage(const std::string& path) {
    texture = std::make_unique<Texture>(renderer->getDevice(), path);
    material = std::make_unique<Material>(texture.get());
    material->properties.color_tint = {1.0f, 1.0f, 1.0f, 1.0f};
    mesh = renderer->getUnitQuad();
}

void
Sprite::setColorTint(const Vec4& color) {
    if (material) material->properties.color_tint = color;
}

void
Sprite::setColor(Vec4 color) {
    setColorTint(color);
}

void
Sprite::setMaterial(Material* material) {
    external_material = material;
}

void
Sprite::setPosition(Vec2 position) {
    this->position = position;
    ui_layout.offset = position;
}

void
Sprite::setScale(Vec2 scale) {
    this->scale = scale;
}

void
Sprite::setSize(Vec2 size) {
    this->size = size;
}

void
Sprite::setRotation(float degree) {
    rotation = lili::degToRad(degree);
}

void
Sprite::setLayer(float layer) {
    this->layer = layer;
}

void
Sprite::setRender(RenderLayer render_layer) {
    this->render_layer = render_layer;
}

void
Sprite::setSlice(SliceUV slice) {
    current_slice = slice;
    Material* mat = getMaterial();
    if (mat) {
        mat->albedoMap = slice.texture;
        mat->properties.uv_bounds = {
            slice.u_min, slice.v_min, slice.u_max, slice.v_max
        };
    }
}

Vec2
Sprite::getPosition() const {
    return position;
}

float
Sprite::getRotation() const {
    return lili::radToDeg(rotation);
}

Vec2
Sprite::getScale() const {
    return scale;
}

float
Sprite::getWidth() const {
    return size.x * scale.x;
}

float
Sprite::getHeight() const {
    return size.y * scale.y;
}

Vec2
Sprite::getSize() const {
    return Vec2(size.x * scale.x, size.y * scale.y);
}

Mat3
Sprite::getTransformMatrix() const {
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

float
Sprite::getLayer() const {
    return layer;
}

RenderLayer
Sprite::getRender() const {
    return render_layer;
}

Vec4
Sprite::getColor() const {
    Material* mat = getMaterial();
    return mat ? mat->properties.color_tint : Vec4(1, 1, 1, 1);
}

Material*
Sprite::getMaterial() const {
    return external_material ? external_material : material.get();
}

void
Sprite::draw() {
    if (!is_visible) return;
    Mat3 mat_transform = getTransformMatrix();
    renderer->submit(
        Model({mesh, getMaterial()}), mat_transform, layer, render_layer
    );
}

}  // namespace lili
