#pragma once

#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/shapes2d.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/passes/pass_types.hpp"
#include "lili2d/render/ui/ui_layout.hpp"

namespace lili {

class Material;
class Renderer;

/// @brief Abstract base interface for all renderable 2D objects.

class IRenderable {
public:
    /// @brief Virtual destructor.
    virtual ~IRenderable() = default;

    /// @brief Submits the object for rendering.
    virtual void
    draw() = 0;

    /// @brief Sets the visibility of the renderable object.
    /// @param visible True to show, false to hide.
    virtual void
    setVisible(bool visible);
    /// @brief Gets the visibility status.
    /// @return True if visible, false otherwise.
    virtual bool
    isVisible() const;

    /// @brief Sets the object position.
    /// @param pos The 2D position vector.
    virtual void
    setPosition(Vec2 pos) = 0;
    /// @brief Gets the object position.
    /// @return The 2D position vector.
    virtual Vec2
    getPosition() const = 0;

    /// @brief Sets rotation in degrees.
    /// @param degree Rotation angle in degrees.
    virtual void
    setRotation(float degree) = 0;
    /// @brief Gets rotation in degrees.
    /// @return Rotation angle in degrees.
    virtual float
    getRotation() const = 0;

    /// @brief Sets scale factors.
    /// @param scale The 2D scale vector.
    virtual void
    setScale(Vec2 scale) = 0;
    /// @brief Gets scale factors.
    /// @return The 2D scale vector.
    virtual Vec2
    getScale() const = 0;

    /// @brief Sets object dimensions (width, height).
    /// @param size The 2D size vector.
    virtual void
    setSize(Vec2 size) = 0;
    /// @brief Gets object dimensions (width, height).
    /// @return The 2D size vector.
    virtual Vec2
    getSize() const = 0;

    /// @brief Computes or returns the transformation matrix.
    /// @return Calculated 3x3 transformation matrix.
    virtual Mat3
    getTransformMatrix() const = 0;

    /// @brief Sets the layer depth within the render pass.
    /// @param layer Depth layer value.
    virtual void
    setLayer(float layer) = 0;
    /// @brief Gets the layer depth.
    /// @return Depth layer value.
    virtual float
    getLayer() const = 0;

    /// @brief Sets the main color tint.
    /// @param color Color RGBA vector.
    virtual void
    setColor(Vec4 color) = 0;
    /// @brief Gets the main color tint.
    /// @return Color RGBA vector.
    virtual Vec4
    getColor() const = 0;

    /// @brief Sets the material used for drawing.
    /// @param material Pointer to Material instance.
    virtual void
    setMaterial(Material* material) = 0;
    /// @brief Gets the material pointer.
    /// @return Pointer to Material instance.
    virtual Material*
    getMaterial() const = 0;

    /// @brief Sets the render pass layer (e.g. WORLD2D, UI).
    /// @param render_layer Render pass layer enum.
    virtual void
    setRender(RenderLayer render_layer);
    /// @brief Gets the render pass layer.
    /// @return Render pass layer enum.
    virtual RenderLayer
    getRender() const;

    /// @brief Sets the UI layout properties.
    /// @param layout UI layout configuration.
    virtual void
    setUILayout(const UILayout& layout);
    /// @brief Gets the UI layout properties.
    /// @return UI layout configuration.
    virtual UILayout
    getUILayout() const;

    /// @brief Convenience setter for UI anchor.
    /// @param anchor UI anchor point.
    virtual void
    setAnchor(Anchor anchor);
    /// @brief Convenience getter for UI anchor.
    /// @return UI anchor point.
    virtual Anchor
    getAnchor() const;

    /// @brief Convenience setter for UI pivot.
    /// @param pivot UI pivot point.
    virtual void
    setPivot(Pivot pivot);
    /// @brief Convenience getter for UI pivot.
    /// @return UI pivot point.
    virtual Pivot
    getPivot() const;

    /// @brief Convenience setter for UI layout offset.
    /// @param offset UI offset vector.
    virtual void
    setOffset(Vec2 offset);
    /// @brief Convenience getter for UI layout offset.
    /// @return UI offset vector.
    virtual Vec2
    getOffset() const;

    /// @brief Gets calculated global screen or world position.
    /// @param renderer Optional pointer to renderer (required for UI swapchain
    /// resolution).
    /// @return Calculated 2D position.
    virtual Vec2
    getGlobalPosition(const Renderer* renderer = nullptr) const;

    /// @brief Gets bounding box rectangle in global coordinates.
    /// @param renderer Optional pointer to renderer.
    /// @return Bounding rectangle.
    virtual RectShape
    getGlobalBounds(const Renderer* renderer = nullptr) const;

    /// @brief Checks if a point (e.g. mouse cursor) intersects this object.
    /// Transforms point into local element coordinates using matrix inversion.
    /// @param point Point to test (e.g. mouse position).
    /// @param renderer Optional pointer to renderer.
    /// @return True if point is inside object bounds.
    virtual bool
    containsPoint(Vec2 point, const Renderer* renderer = nullptr) const;

protected:
    /// @brief Flag indicating if object is visible.
    bool is_visible = true;
    /// @brief UI layout configuration data.
    UILayout ui_layout;
    /// @brief The render layer
    RenderLayer render_layer = RenderLayer::WORLD2D;
};

}  // namespace lili
