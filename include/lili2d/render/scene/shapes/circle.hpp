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

/// @brief A renderable circle.
class Circle : public IRenderable {
public:
    /// @brief Default constructor.
    Circle() = default;
    /// @brief Constructs a renderable circle.
    /// @param renderer The renderer.
    /// @param shape The circle geometry.
    /// @param color The color.
    Circle(Renderer* renderer, CircleShape shape, Vec4 color);
    /// @brief Default destructor.
    ~Circle() override = default;

    /// @brief Move constructor.
    Circle(Circle&&) = default;
    /// @brief Move assignment operator.
    /// @return Reference to the assigned circle.
    Circle&
    operator=(Circle&&) = default;

    /// @brief Sets the center position.
    /// @param pos The new center.
    void
    setPosition(Vec2 pos) override;
    /// @brief Sets rotation in degrees.
    /// @param degree Rotation in degrees.
    void
    setRotation(float degree) override;
    /// @brief Sets scale factors.
    /// @param scale The new scale.
    void
    setScale(Vec2 scale) override;
    /// @brief Sets size (width, height) by setting radius = max(w,h)/2.
    /// @param size The new size.
    void
    setSize(Vec2 size) override;

    /// @brief Sets the center position.
    /// @param pos The new center.
    void
    setCenter(Vec2 pos);
    /// @brief Sets the radius.
    /// @param r The new radius.
    void
    setRadius(float r);
    /// @brief Sets the number of segments.
    /// @param n The new number of segments.
    void
    setSegments(int n);
    /// @brief Sets the circle shape.
    /// @param shape The new shape.
    void
    setShape(CircleShape shape);
    /// @brief Sets the color.
    /// @param color The new color.
    void
    setColor(Vec4 color) override;
    /// @brief Sets the material.
    /// @param material The new material.
    void
    setMaterial(Material* material) override;
    /// @brief Sets whether the circle is hollow.
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
    /// @brief Sets the render pass layer.
    /// @param render_layer The new render pass layer.
    void
    setRender(RenderLayer render_layer) override;

    /// @brief Gets position (center).
    /// @return The center position.
    Vec2
    getPosition() const override;
    /// @brief Gets rotation in degrees.
    /// @return Rotation in degrees.
    float
    getRotation() const override;
    /// @brief Gets scale.
    /// @return The scale.
    Vec2
    getScale() const override;
    /// @brief Gets size (diameter, diameter).
    /// @return The size.
    Vec2
    getSize() const override;
    /// @brief Gets the transformation matrix.
    /// @return The Mat3 transform matrix.
    Mat3
    getTransformMatrix() const override;
    /// @brief Gets layer depth.
    /// @return The depth.
    float
    getLayer() const override;
    /// @brief Gets render pass layer.
    /// @return The render layer.
    RenderLayer
    getRender() const override;

    /// @brief Checks if a point is inside the circle.
    /// @param point Point to test.
    /// @param renderer Optional renderer pointer.
    /// @return True if point is inside circle radius.
    bool
    containsPoint(
        Vec2 point, const Renderer* renderer = nullptr
    ) const override;

    /// @brief Gets the center position.
    /// @return The center position.
    Vec2
    getCenter() const;
    /// @brief Gets the top-left position (center - radius).
    /// @return The top-left position.
    Vec2
    getTopLeft() const;
    /// @brief Gets the radius.
    /// @return The radius.
    float
    getRadius() const;
    /// @brief Gets the diameter.
    /// @return The diameter.
    float
    getDiameter() const;
    /// @brief Gets the shape geometry.
    /// @return The shape.
    CircleShape
    getShape() const;
    /// @brief Gets the color.
    /// @return The color.
    Vec4
    getColor() const override;
    /// @brief Gets the material.
    /// @return Pointer to the material.
    Material*
    getMaterial() const override;
    /// @brief Returns whether the circle is hollow.
    /// @return True if hollow, false otherwise.
    bool
    isHollow() const;
    /// @brief Gets the hollow thickness.
    /// @return The hollow outline thickness.
    float
    getHollowThickness() const;

    /// @brief Submits the circle for drawing.
    void
    draw() override;

private:
    /// @brief Pointer to parent Renderer.
    Renderer* renderer = nullptr;
    /// @brief Circle shape geometry.
    CircleShape shape;
    /// @brief Rotation angle in degrees.
    float rotation = 0.0f;
    /// @brief 2D scale vector.
    Vec2 scale = {1.0f, 1.0f};
    /// @brief Flag indicating if circle is drawn hollow.
    bool is_hollow = false;
    /// @brief Outline thickness when hollow.
    float hollow_thickness = 1.0f;

    /// @brief Render layer depth.
    float layer = 0.0f;
    /// @brief Target render pass layer.
    RenderLayer render_layer = RenderLayer::WORLD2D;
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
