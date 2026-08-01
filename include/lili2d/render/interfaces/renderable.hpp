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
    virtual void
    setVisible(bool visible);
    /// @brief Gets the visibility status.
    virtual bool
    isVisible() const;

    /// @brief Sets the object position.
    virtual void
    setPosition(Vec2 pos) = 0;
    /// @brief Gets the object position.
    virtual Vec2
    getPosition() const = 0;

    /// @brief Sets rotation in degrees.
    virtual void
    setRotation(float degree) = 0;
    /// @brief Gets rotation in degrees.
    virtual float
    getRotation() const = 0;

    /// @brief Sets scale factors.
    virtual void
    setScale(Vec2 scale) = 0;
    /// @brief Gets scale factors.
    virtual Vec2
    getScale() const = 0;

    /// @brief Sets object dimensions (width, height).
    virtual void
    setSize(Vec2 size) = 0;
    /// @brief Gets object dimensions (width, height).
    virtual Vec2
    getSize() const = 0;

    /// @brief Computes or returns the transformation matrix.
    virtual Mat3
    getTransformMatrix() const = 0;

    /// @brief Sets the layer depth within the render pass.
    virtual void
    setLayer(float layer) = 0;
    /// @brief Gets the layer depth.
    virtual float
    getLayer() const = 0;

    /// @brief Sets the render pass layer (e.g. WORLD2D, UI).
    virtual void
    setRender(RenderLayer render_layer) = 0;
    /// @brief Gets the render pass layer.
    virtual RenderLayer
    getRender() const = 0;

    /// @brief Sets the main color tint.
    virtual void
    setColor(Vec4 color) = 0;
    /// @brief Gets the main color tint.
    virtual Vec4
    getColor() const = 0;

    /// @brief Sets the material used for drawing.
    virtual void
    setMaterial(Material* material) = 0;
    /// @brief Gets the material pointer.
    virtual Material*
    getMaterial() const = 0;

    /// @brief Sets the UI layout properties.
    virtual void
    setUILayout(const UILayout& layout);
    /// @brief Gets the UI layout properties.
    virtual UILayout
    getUILayout() const;

    /// @brief Convenience setter for UI anchor.
    virtual void
    setAnchor(Anchor anchor);
    /// @brief Convenience getter for UI anchor.
    virtual Anchor
    getAnchor() const;

    /// @brief Convenience setter for UI pivot.
    virtual void
    setPivot(Pivot pivot);
    /// @brief Convenience getter for UI pivot.
    virtual Pivot
    getPivot() const;

    /// @brief Convenience setter for UI layout offset.
    virtual void
    setOffset(Vec2 offset);
    /// @brief Convenience getter for UI layout offset.
    virtual Vec2
    getOffset() const;

    /// @brief Gets calculated global screen or world position.
    /// @param renderer Optional pointer to renderer (required for UI swapchain resolution).
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
    bool is_visible = true;
    UILayout ui_layout;
};

}  // namespace lili

