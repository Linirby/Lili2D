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
    Line() = default;
    /// @brief Constructs a renderable line.
    /// @param renderer The renderer.
    /// @param shape The line geometry.
    /// @param color The color.
    explicit Line(Renderer* renderer, LineShape shape, Vec4 color);
    /// @brief Default destructor.
    ~Line() override = default;

    /// @brief Move constructor.
    Line(Line&&) = default;
    /// @brief Move assignment operator.
    /// @return Reference to the assigned line.
    Line&
    operator=(Line&&) = default;

    /// @brief Sets start position.
    /// @param pos New start position.
    void
    setPosition(Vec2 pos) override;
    /// @brief Sets rotation in degrees (rotates end around start).
    /// @param degree Angle in degrees.
    void
    setRotation(float degree) override;
    /// @brief Sets scale factors.
    /// @param scale The new scale.
    void
    setScale(Vec2 scale) override;
    /// @brief Sets size (length = size.x, thickness = size.y).
    /// @param size The new size.
    void
    setSize(Vec2 size) override;

    /// @brief Sets the start position.
    /// @param pos The new start position.
    void
    setStart(Vec2 pos);
    /// @brief Sets the end position.
    /// @param pos The new end position.
    void
    setEnd(Vec2 pos);
    /// @brief Sets the thickness.
    /// @param value The new thickness.
    void
    setThickness(float value);
    /// @brief Sets the line shape.
    /// @param shape The new shape.
    void
    setShape(LineShape shape);
    /// @brief Sets the color.
    /// @param color The new color.
    void
    setColor(Vec4 color) override;
    /// @brief Sets the material pointer.
    /// @param material Material pointer.
    void
    setMaterial(Material* material) override;
    /// @brief Sets the depth value for Z-ordering.
    /// @param value The new layer depth.
    void
    setLayer(float value) override;

    /// @brief Gets start position.
    /// @return Start position.
    Vec2
    getPosition() const override;
    /// @brief Gets rotation angle in degrees.
    /// @return Rotation angle in degrees.
    float
    getRotation() const override;
    /// @brief Gets scale.
    /// @return Scale vector.
    Vec2
    getScale() const override;
    /// @brief Gets bounding size (length * scale.x, thickness * scale.y).
    /// @return Size vector.
    Vec2
    getSize() const override;
    /// @brief Gets the transformation matrix.
    /// @return Mat3 matrix.
    Mat3
    getTransformMatrix() const override;
    /// @brief Gets layer depth.
    /// @return Depth layer.
    float
    getLayer() const override;
    /// @brief Gets the start position.
    /// @return The start position.
    Vec2
    getStart() const;
    /// @brief Gets the end position.
    /// @return The end position.
    Vec2
    getEnd() const;
    /// @brief Gets the thickness.
    /// @return The thickness.
    float
    getThickness() const;
    /// @brief Gets the shape geometry.
    /// @return The shape.
    LineShape
    getShape() const;
    /// @brief Gets the color.
    /// @return The color.
    Vec4
    getColor() const override;
    /// @brief Gets the material.
    /// @return Pointer to the material.
    Material*
    getMaterial() const override;

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
