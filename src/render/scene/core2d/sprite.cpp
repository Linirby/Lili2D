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
    material->properties.color_tint = Vec4(1.0f, 1.0f, 1.0f, 1.0f);

    position = Vec2(0.0f, 0.0f);
    scale = Vec2(1.0f, 1.0f);
    size = Vec2(texture->getWidth(), texture->getHeight());
    rotation = 0.0f;
    layer = 0.0f;
}

Sprite::Sprite(Renderer* renderer, const SliceUV& slice) : renderer(renderer) {
    mesh = renderer->getUnitQuad();

    material = std::make_unique<Material>(slice.texture);
    material->properties.color_tint = Vec4(1.0f, 1.0f, 1.0f, 1.0f);

    position = Vec2(0.0f, 0.0f);
    scale = Vec2(1.0f, 1.0f);
    size = Vec2(slice.width, slice.height);
    rotation = 0.0f;
    layer = 0.0f;
    setSlice(slice);
}

Sprite::Sprite(Renderer* renderer, Texture* texture) : renderer(renderer) {
    mesh = renderer->getUnitQuad();

    material = std::make_unique<Material>(texture);
    material->properties.color_tint = Vec4(1.0f, 1.0f, 1.0f, 1.0f);

    position = Vec2(0.0f, 0.0f);
    scale = Vec2(1.0f, 1.0f);
    if (texture) size = Vec2(texture->getWidth(), texture->getHeight());
    rotation = 0.0f;
    layer = 0.0f;
}

void
Sprite::setImage(const std::string& path) {
    texture = std::make_unique<Texture>(renderer->getDevice(), path);
    material = std::make_unique<Material>(texture.get());
    material->properties.color_tint = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    mesh = renderer->getUnitQuad();
}

void
Sprite::setSlice(const SliceUV& slice) {
    current_slice = slice;
    Material* mat = getMaterial();
    if (mat) {
        mat->albedoMap = slice.texture;
        mat->properties.uv_bounds =
            Vec4(slice.u_min, slice.v_min, slice.u_max, slice.v_max);
    }
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

void
Sprite::draw() {
    if (!is_visible) return;
    Mat3 mat_transform = getTransformMatrix();
    renderer->submit(
        Model({mesh, getMaterial()}), mat_transform, layer, render_layer
    );
}

}  // namespace lili
