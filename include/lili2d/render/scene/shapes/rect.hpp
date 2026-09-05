#pragma once

#include <memory>

#include "lili2d/geometry/shapes2d.hpp"
#include "lili2d/geometry/utils.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/core/gpu_mesh.hpp"
#include "lili2d/render/interfaces/renderable.hpp"
#include "lili2d/render/renderer.hpp"
#include "lili2d/render/scene/common/material.hpp"

namespace lili {

/// @brief A renderable rectangle.
class Rect : public IRenderable {
public:
    /// @brief Default constructor.
    Rect() noexcept = default;
    /// @brief Constructs a renderable rectangle.
    /// @param renderer The renderer.
    /// @param shape The rectangle geometry.
    /// @param color The color of the rectangle.
    explicit Rect(Renderer* renderer, RectShape shape, Vec4 color);
    /// @brief Default destructor.
    ~Rect() override = default;

    /// @brief Move constructor.
    Rect(Rect&&) noexcept = default;
    /// @brief Move assignment operator.
    /// @return Reference to the assigned rectangle.
    Rect&
    operator=(Rect&&) noexcept = default;

    /// @brief Sets the rectangle's position.
    /// @param pos The new position.
    inline void
    setPosition(Vec2 pos) noexcept override {
        shape.pos = pos;
        ui_layout.offset = pos;
    }

    /// @brief Sets the rectangle's size.
    /// @param size The new size.
    void
    setSize(Vec2 size) noexcept override;

    /// @brief Sets the rectangle's rotation.
    /// @param degree The rotation in degrees.
    inline void
    setRotation(float degree) noexcept override {
        rotation = lili::degToRad(degree);
    }

    /// @brief Sets scale factors.
    /// @param scale The new scale.
    inline void
    setScale(Vec2 scale) noexcept override {
        this->scale = scale;
    }

    /// @brief Sets the rectangle's shape.
    /// @param shape The new shape.
    void
    setShape(RectShape shape) noexcept;

    /// @brief Sets the rectangle's color.
    /// @param color The new color.
    inline void
    setColor(Vec4 color) noexcept override {
        if (material) {
            material->properties.color_tint = color;
        }
    }

    /// @brief Sets the material.
    /// @param mat The new material.
    inline void
    setMaterial(Material* mat) noexcept override {
        external_material = mat;
    }

    /// @brief Sets whether the rectangle is hollow.
    /// @param hollow The new hollow state.
    inline void
    setHollow(bool hollow) noexcept {
        is_hollow = hollow;
    }

    /// @brief Sets the outline thickness when hollow.
    /// @param thickness The thickness of the outline.
    inline void
    setHollowThickness(float thickness) noexcept {
        if (hollow_thickness != thickness) {
            hollow_thickness = thickness;
            hollow_dirty = true;
        }
    }

    /// @brief Sets the depth value for Z-ordering.
    /// @param value The new layer depth.
    inline void
    setLayer(float value) noexcept override {
        layer = value;
    }

    /// @brief Gets the position.
    /// @return The position.
    [[nodiscard]] inline Vec2
    getPosition() const noexcept override {
        return shape.pos;
    }

    /// @brief Gets the size.
    /// @return The size.
    [[nodiscard]] inline Vec2
    getSize() const noexcept override {
        return shape.size;
    }

    /// @brief Gets rotation in degrees.
    /// @return Rotation in degrees.
    [[nodiscard]] inline float
    getRotation() const noexcept override {
        return lili::radToDeg(rotation);
    }

    /// @brief Gets scale factors.
    /// @return The scale.
    [[nodiscard]] inline Vec2
    getScale() const noexcept override {
        return scale;
    }

    /// @brief Gets the transformation matrix.
    /// @return The Mat3 transform matrix.
    [[nodiscard]] Mat3
    getTransformMatrix() const override;

    /// @brief Gets the layer depth.
    /// @return The depth.
    [[nodiscard]] inline float
    getLayer() const noexcept override {
        return layer;
    }

    /// @brief Gets the shape geometry.
    /// @return The shape.
    [[nodiscard]] inline RectShape
    getShape() const noexcept {
        return shape;
    }

    /// @brief Gets the color.
    /// @return The color.
    [[nodiscard]] inline Vec4
    getColor() const noexcept override {
        Material* mat = getMaterial();
        return mat ? mat->properties.color_tint : Vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    /// @brief Gets the material.
    /// @return Pointer to the material.
    [[nodiscard]] inline Material*
    getMaterial() const noexcept override {
        return external_material ? external_material : material.get();
    }

    /// @brief Returns whether the rectangle is hollow.
    /// @return True if hollow, false otherwise.
    [[nodiscard]] inline bool
    isHollow() const noexcept {
        return is_hollow;
    }

    /// @brief Gets the hollow thickness.
    /// @return The hollow outline thickness.
    [[nodiscard]] inline float
    getHollowThickness() const noexcept {
        return hollow_thickness;
    }

    /// @brief Submits the rectangle for drawing.
    void
    draw() override;

private:
    Renderer* renderer = nullptr;
    RectShape shape;
    float rotation = 0.0f;
    Vec2 scale = {1.0f, 1.0f};
    bool is_hollow = false;
    float hollow_thickness = 1.0f;

    float layer = 0.0f;
    GPUMesh* mesh = nullptr;
    std::unique_ptr<Material> material = nullptr;
    Material* external_material = nullptr;
    std::unique_ptr<GPUMesh> hollow_mesh = nullptr;
    bool hollow_dirty = true;
};

}  // namespace lili
