#pragma once

#include <memory>

#include "lili2d/geometry/shapes2d.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/core/gpu_mesh.hpp"
#include "lili2d/render/interfaces/renderable.hpp"
#include "lili2d/render/renderer.hpp"
#include "lili2d/render/scene/common/material.hpp"

namespace lili {

/// @brief A renderable line.
class Line : public IRenderable {
public:
    /// @brief Default constructor.
    Line() noexcept = default;
    /// @brief Constructs a renderable line.
    /// @param renderer The renderer.
    /// @param shape The line geometry.
    /// @param color The color.
    explicit Line(Renderer* renderer, LineShape shape, Vec4 color);
    /// @brief Default destructor.
    ~Line() override = default;

    /// @brief Move constructor.
    Line(Line&&) noexcept = default;
    /// @brief Move assignment operator.
    /// @return Reference to the assigned line.
    Line&
    operator=(Line&&) noexcept = default;

    /// @brief Sets start position.
    /// @param pos New start position.
    void
    setPosition(Vec2 pos) noexcept override;
    /// @brief Sets rotation in degrees (rotates end around start).
    /// @param degree Angle in degrees.
    void
    setRotation(float degree) noexcept override;
    /// @brief Sets scale factors.
    /// @param scale The new scale.
    inline void
    setScale(Vec2 scale) noexcept override {
        this->scale = scale;
    }
    /// @brief Sets size (length = size.x, thickness = size.y).
    /// @param size The new size.
    void
    setSize(Vec2 size) noexcept override;

    /// @brief Sets the start position.
    /// @param pos The new start position.
    inline void
    setStart(Vec2 pos) noexcept {
        shape.start = pos;
        ui_layout.offset = pos;
    }

    /// @brief Sets the end position.
    /// @param pos The new end position.
    inline void
    setEnd(Vec2 pos) noexcept {
        shape.end = pos;
    }

    /// @brief Sets the thickness.
    /// @param value The new thickness.
    inline void
    setThickness(float value) noexcept {
        shape.thickness = value;
    }

    /// @brief Sets the line shape.
    /// @param shape The new shape.
    inline void
    setShape(LineShape shape) noexcept {
        this->shape = shape;
        ui_layout.offset = shape.start;
    }

    /// @brief Sets the color.
    /// @param color The new color.
    inline void
    setColor(Vec4 color) noexcept override {
        if (material) {
            material->properties.color_tint = color;
        }
    }

    /// @brief Sets the material pointer.
    /// @param mat Material pointer.
    inline void
    setMaterial(Material* mat) noexcept override {
        external_material = mat;
    }

    /// @brief Sets the depth value for Z-ordering.
    /// @param value The new layer depth.
    inline void
    setLayer(float value) noexcept override {
        layer = value;
    }

    /// @brief Gets start position.
    /// @return Start position.
    [[nodiscard]] inline Vec2
    getPosition() const noexcept override {
        return shape.start;
    }

    /// @brief Gets rotation angle in degrees.
    /// @return Rotation angle in degrees.
    [[nodiscard]] float
    getRotation() const noexcept override;

    /// @brief Gets scale.
    /// @return Scale vector.
    [[nodiscard]] inline Vec2
    getScale() const noexcept override {
        return scale;
    }

    /// @brief Gets bounding size (length * scale.x, thickness * scale.y).
    /// @return Size vector.
    [[nodiscard]] Vec2
    getSize() const noexcept override;

    /// @brief Gets the transformation matrix.
    /// @return Mat3 matrix.
    [[nodiscard]] Mat3
    getTransformMatrix() const override;

    /// @brief Gets layer depth.
    /// @return The depth.
    [[nodiscard]] inline float
    getLayer() const noexcept override {
        return layer;
    }

    /// @brief Gets start position.
    /// @return Start position.
    [[nodiscard]] inline Vec2
    getStart() const noexcept {
        return shape.start;
    }

    /// @brief Gets end position.
    /// @return End position.
    [[nodiscard]] inline Vec2
    getEnd() const noexcept {
        return shape.end;
    }

    /// @brief Gets thickness.
    /// @return The thickness.
    [[nodiscard]] inline float
    getThickness() const noexcept {
        return shape.thickness;
    }

    /// @brief Gets line shape geometry.
    /// @return The shape.
    [[nodiscard]] inline LineShape
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

    /// @brief Submits the line for drawing.
    void
    draw() override;

private:
    /// @brief Pointer to parent Renderer.
    Renderer* renderer = nullptr;
    /// @brief Line shape geometry.
    LineShape shape;
    /// @brief 2D scale vector.
    Vec2 scale = {1.0f, 1.0f};

    /// @brief Render layer depth.
    float layer = 0.0f;
    /// @brief Pointer to GPU mesh.
    GPUMesh* mesh = nullptr;
    /// @brief Unique pointer to owned Material.
    std::unique_ptr<Material> material = nullptr;
    /// @brief Pointer to external unowned Material.
    Material* external_material = nullptr;
};

}  // namespace lili
