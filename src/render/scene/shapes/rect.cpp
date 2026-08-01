#include "lili2d/render/scene/shapes/rect.hpp"

#include "lili2d/geometry/utils.hpp"
#include "lili2d/geometry/vec3.hpp"

namespace lili {

Rect::Rect(Renderer* renderer, RectShape shape, Vec4 color)

    : renderer(renderer) {
    mesh = renderer->getUnitQuad();
    material = std::make_unique<Material>(renderer->getTheWhitePixel());
    setShape(shape);
    setColor(color);
    ui_layout.offset = {shape.x, shape.y};
    rotation = 0.0f;
    layer = 0.0f;
}

void
Rect::setPosition(Vec2 pos) {
    shape.x = pos.x;
    shape.y = pos.y;
    ui_layout.offset = pos;
}

void
Rect::setRotation(float degree) {
    rotation = lili::degToRad(degree);
}

void
Rect::setScale(Vec2 scale) {
    this->scale = scale;
}

void
Rect::setSize(Vec2 size) {
    if (shape.w != size.x || shape.h != size.y) {
        shape.w = size.x;
        shape.h = size.y;
        hollow_dirty = true;
    }
}

void
Rect::setShape(RectShape shape) {
    if (this->shape.w != shape.w || this->shape.h != shape.h) {
        hollow_dirty = true;
    }
    this->shape = shape;
    ui_layout.offset = {shape.x, shape.y};
}

void
Rect::setColor(Vec4 color) {
    if (material) {
        material->properties.color_tint = color;
    }
}

void
Rect::setMaterial(Material* material) {
    external_material = material;
}

void
Rect::setHollow(bool hollow) {
    is_hollow = hollow;
}

void
Rect::setHollowThickness(float thickness) {
    if (hollow_thickness != thickness) {
        hollow_thickness = thickness;
        hollow_dirty = true;
    }
}

void
Rect::setLayer(float value) {
    layer = value;
}

void
Rect::setRender(RenderLayer render_layer) {
    this->render_layer = render_layer;
}

Vec2
Rect::getPosition() const {
    return {shape.x, shape.y};
}

Vec2
Rect::getSize() const {
    return {shape.w, shape.h};
}

float
Rect::getRotation() const {
    return lili::radToDeg(rotation);
}

Vec2
Rect::getScale() const {
    return scale;
}

Mat3
Rect::getTransformMatrix() const {
    if (render_layer == RenderLayer::UI && renderer) {
        Vec2 viewport_size = {
            static_cast<float>(renderer->getSwapchainWidth()),
            static_cast<float>(renderer->getSwapchainHeight())
        };
        Vec2 obj_size = {shape.w * scale.x, shape.h * scale.y};
        return ui_layout.getTransformationMatrix(
            viewport_size, obj_size, rotation, obj_size
        );
    }
    return Mat3::translate({shape.x, shape.y}) * Mat3::rotation(rotation) *
           Mat3::scale({shape.w * scale.x, shape.h * scale.y});
}


float
Rect::getLayer() const {
    return layer;
}

RenderLayer
Rect::getRender() const {
    return render_layer;
}

RectShape
Rect::getShape() const {
    return shape;
}

Vec4
Rect::getColor() const {
    Material* mat = getMaterial();
    return mat ? mat->properties.color_tint : Vec4(1, 1, 1, 1);
}

Material*
Rect::getMaterial() const {
    return external_material ? external_material : material.get();
}

bool
Rect::isHollow() const {
    return is_hollow;
}

float
Rect::getHollowThickness() const {
    return hollow_thickness;
}

void
Rect::draw() {
    if (!is_visible) return;

    Mat3 mat_transform = getTransformMatrix();

    if (is_hollow) {
        if (hollow_dirty) {
            float w = (shape.w > 0.0001f) ? shape.w : 1.0f;
            float h = (shape.h > 0.0001f) ? shape.h : 1.0f;
            float tx = std::min(0.5f, hollow_thickness / w);
            float ty = std::min(0.5f, hollow_thickness / h);

            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;

            auto add_quad = [&](float qx, float qy, float qw, float qh) {
                uint32_t start_idx = vertices.size();
                vertices.push_back(Vertex{qx, qy, 0.0f, qx, qy});
                vertices.push_back(Vertex{qx + qw, qy, 0.0f, qx + qw, qy});
                vertices.push_back(Vertex{qx, qy + qh, 0.0f, qx, qy + qh});
                vertices.push_back(
                    Vertex{qx + qw, qy + qh, 0.0f, qx + qw, qy + qh}
                );

                indices.push_back(start_idx + 0);
                indices.push_back(start_idx + 1);
                indices.push_back(start_idx + 2);
                indices.push_back(start_idx + 2);
                indices.push_back(start_idx + 1);
                indices.push_back(start_idx + 3);
            };

            // Build normalized unit quad ring [0..1] x [0..1]
            add_quad(0.0f, 0.0f, 1.0f, ty);       // Top bar
            add_quad(0.0f, 1.0f - ty, 1.0f, ty);  // Bottom bar
            float inner_h = 1.0f - 2.0f * ty;
            if (inner_h > 0.0f) {
                add_quad(0.0f, ty, tx, inner_h);       // Left bar
                add_quad(1.0f - tx, ty, tx, inner_h);  // Right bar
            }

            MeshData mesh_data;
            mesh_data.vertices = vertices;
            mesh_data.indices = indices;
            if (!hollow_mesh)
                hollow_mesh =
                    std::make_unique<GPUMesh>(renderer->getDevice(), mesh_data);
            else
                hollow_mesh->update(mesh_data);
            hollow_dirty = false;
        }

        renderer->submit(
            Model(hollow_mesh.get(), getMaterial()), mat_transform, layer,
            render_layer
        );
    } else
        renderer->submit(
            Model(mesh, getMaterial()), mat_transform, layer, render_layer
        );
}

}  // namespace lili
