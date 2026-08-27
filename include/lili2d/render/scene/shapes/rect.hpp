#pragma once

#include <memory>

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
    Rect() = default;
    /// @brief Constructs a renderable rectangle.
    /// @param renderer The renderer.
    /// @param shape The rectangle geometry.
    /// @param color The color of the rectangle.
    explicit Rect(Renderer* renderer, RectShape shape, Vec4 color);
    /// @brief Default destructor.
    ~Rect() override = default;

    /// @brief Move constructor.
    Rect(Rect&&) = default;
    /// @brief Move assignment operator.
    /// @return Reference to the assigned rectangle.
    Rect&
    operator=(Rect&&) = default;

    /// @brief Sets the rectangle's position.
    /// @param pos The new position.
    void
    setPosition(Vec2 pos) override;
    /// @brief Sets the rectangle's size.
    /// @param size The new size.
    void
    setSize(Vec2 size) override;
    /// @brief Sets the rectangle's rotation.
    /// @param degree The rotation in degrees.
    void
    setRotation(float degree) override;
    /// @brief Sets scale factors.
    /// @param scale The new scale.
    void
    setScale(Vec2 scale) override;
    /// @brief Sets the rectangle's shape.
    /// @param shape The new shape.
    void
    setShape(RectShape shape);
    /// @brief Sets the rectangle's color.
    /// @param color The new color.
    void
    setColor(Vec4 color) override;
    /// @brief Sets the material.
    /// @param material The new material.
    void
    setMaterial(Material* material) override;
    /// @brief Sets whether the rectangle is hollow.
    /// @param hollow The new hollow state.
    void
    setHollow(bool hollow);
    /// @brief Sets the outline thickness when hollow.
    /// @param thickness The thickness of the outline.
    void
    setHollowThickness(float thickness);
    /// @brief Sets the depth value for Z-ordering.
    /// @param value The new layer depth.
    void
    setLayer(float value) override;

    /// @brief Gets the position.
    /// @return The position.
    Vec2
    getPosition() const override;
    /// @brief Gets the size.
    /// @return The size.
    Vec2
    getSize() const override;
    /// @brief Gets rotation in degrees.
    /// @return Rotation in degrees.
    float
    getRotation() const override;
    /// @brief Gets scale factors.
    /// @return The scale.
    Vec2
    getScale() const override;
    /// @brief Gets the transformation matrix.
    /// @return The Mat3 transform matrix.
    Mat3
    getTransformMatrix() const override;
    /// @brief Gets the layer depth.
    /// @return The depth.
    float
    getLayer() const override;
    /// @brief Gets the shape geometry.
    /// @return The shape.
    RectShape
    getShape() const;
    /// @brief Gets the color.
    /// @return The color.
    Vec4
    getColor() const override;
    /// @brief Gets the material.
    /// @return Pointer to the material.
    Material*
    getMaterial() const override;
    /// @brief Returns whether the rectangle is hollow.
    /// @return True if hollow, false otherwise.
    bool
    isHollow() const;
    /// @brief Gets the hollow thickness.
    /// @return The hollow outline thickness.
    float
    getHollowThickness() const;

    /// @brief Submits the rectangle for drawing.
    void
    draw() override;

private:
    /// @brief Pointer to parent Renderer.
    Renderer* renderer = nullptr;
    /// @brief Rectangle shape geometry.
    RectShape shape;
    /// @brief Rotation angle in degrees.
    float rotation = 0.0f;
    /// @brief 2D scale vector.
    Vec2 scale = {1.0f, 1.0f};
    /// @brief Flag indicating if rectangle is drawn hollow.
    bool is_hollow = false;
    /// @brief Outline thickness when hollow.
    float hollow_thickness = 1.0f;

    /// @brief Render layer depth.
    float layer = 0.0f;
    /// @brief Pointer to filled GPU mesh.
    GPUMesh* mesh = nullptr;
    /// @brief Unique pointer to owned Material.
    std::unique_ptr<Material> material = nullptr;
    /// @brief Pointer to external unowned Material.
    Material* external_material = nullptr;
    /// @brief Unique pointer to hollow outline GPU mesh.
    std::unique_ptr<GPUMesh> hollow_mesh = nullptr;
    /// @brief Flag indicating if hollow mesh needs rebuilding.
    bool hollow_dirty = true;
};

}  // namespace lili
