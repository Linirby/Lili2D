#pragma once

#include <algorithm>
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

/// @brief A renderable circle.
class Circle : public IRenderable {
public:
    /// @brief Default constructor.
    Circle() noexcept = default;
    /// @brief Constructs a renderable circle.
    /// @param renderer The renderer.
    /// @param shape The circle geometry.
    /// @param color The color.
    explicit Circle(Renderer* renderer, CircleShape shape, Vec4 color);
    /// @brief Default destructor.
    ~Circle() override = default;

    /// @brief Move constructor.
    Circle(Circle&&) noexcept = default;
    /// @brief Move assignment operator.
    /// @return Reference to the assigned circle.
    Circle&
    operator=(Circle&&) noexcept = default;

    /// @brief Sets the center position.
    /// @param pos The new center.
    inline void
    setPosition(Vec2 pos) noexcept override {
        shape.center = pos;
        ui_layout.offset = pos;
    }

    /// @brief Sets rotation in degrees.
    /// @param degree Rotation in degrees.
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

    /// @brief Sets size (width, height) by setting radius = max(w,h)/2.
    /// @param size The new size.
    inline void
    setSize(Vec2 size) noexcept override {
        float max_dim = std::max(size.x, size.y);
        setRadius(max_dim * 0.5f);
    }

    /// @brief Sets the center position.
    /// @param pos The new center.
    inline void
    setCenter(Vec2 pos) noexcept {
        shape.center = pos;
        ui_layout.offset = pos;
    }

    /// @brief Sets the radius.
    /// @param r The new radius.
    inline void
    setRadius(float r) noexcept {
        if (shape.radius != r) {
            shape.radius = r;
            hollow_dirty = true;
        }
    }

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
    inline void
    setColor(Vec4 color) noexcept override {
        if (material) material->properties.color_tint = color;
    }

    /// @brief Sets the material.
    /// @param mat The new material.
    inline void
    setMaterial(Material* mat) noexcept override {
        external_material = mat;
    }

    /// @brief Sets whether the circle is hollow.
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

    /// @brief Gets position (center).
    /// @return The center position.
    [[nodiscard]] inline Vec2
    getPosition() const noexcept override {
        return shape.center;
    }

    /// @brief Gets rotation in degrees.
    /// @return Rotation in degrees.
    [[nodiscard]] inline float
    getRotation() const noexcept override {
        return lili::radToDeg(rotation);
    }

    /// @brief Gets scale.
    /// @return The scale.
    [[nodiscard]] inline Vec2
    getScale() const noexcept override {
        return scale;
    }

    /// @brief Gets size (diameter, diameter).
    /// @return The size.
    [[nodiscard]] inline Vec2
    getSize() const noexcept override {
        float d = getDiameter();
        return {d, d};
    }

    /// @brief Gets the transformation matrix.
    /// @return The Mat3 transform matrix.
    [[nodiscard]] Mat3
    getTransformMatrix() const override;

    /// @brief Gets layer depth.
    /// @return The depth.
    [[nodiscard]] inline float
    getLayer() const noexcept override {
        return layer;
    }

    /// @brief Checks if a point is inside the circle.
    /// @param point Point to test.
    /// @param renderer Optional renderer pointer.
    /// @return True if point is inside circle radius.
    [[nodiscard]] bool
    containsPoint(
        Vec2 point, const Renderer* renderer = nullptr
    ) const override;

    /// @brief Gets the center position.
    /// @return The center position.
    [[nodiscard]] inline Vec2
    getCenter() const noexcept {
        return shape.center;
    }

    /// @brief Gets the top-left position (center - radius).
    /// @return The top-left position.
    [[nodiscard]] inline Vec2
    getTopLeft() const noexcept {
        return shape.center - Vec2(shape.radius, shape.radius);
    }

    /// @brief Gets the radius.
    /// @return The radius.
    [[nodiscard]] inline float
    getRadius() const noexcept {
        return shape.radius;
    }

    /// @brief Gets the diameter.
    /// @return The diameter.
    [[nodiscard]] inline float
    getDiameter() const noexcept {
        return shape.radius * 2.0f;
    }

    /// @brief Gets the shape geometry.
    /// @return The shape.
    [[nodiscard]] inline CircleShape
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

    /// @brief Returns whether the circle is hollow.
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

    /// @brief Submits the circle for drawing.
    void
    draw() override;

private:
    Renderer* renderer = nullptr;
    CircleShape shape;
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
