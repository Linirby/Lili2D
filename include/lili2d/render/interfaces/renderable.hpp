#pragma once

#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/shapes2d.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/passes/pass_types.hpp"
#include "lili2d/render/ui/ui_layout.hpp"

namespace lili {

struct Material;
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
    setVisible(bool visible) noexcept;
    /// @brief Gets the visibility status.
    /// @return True if visible, false otherwise.
    [[nodiscard]] virtual bool
    isVisible() const noexcept;

    /// @brief Sets the object position.
    /// @param pos The 2D position vector.
    virtual void
    setPosition(Vec2 pos) noexcept = 0;
    /// @brief Gets the object position.
    /// @return The 2D position vector.
    [[nodiscard]] virtual Vec2
    getPosition() const noexcept = 0;

    /// @brief Sets rotation in degrees.
    /// @param degree Rotation angle in degrees.
    virtual void
    setRotation(float degree) noexcept = 0;
    /// @brief Gets rotation in degrees.
    /// @return Rotation angle in degrees.
    [[nodiscard]] virtual float
    getRotation() const noexcept = 0;

    /// @brief Sets scale factors.
    /// @param scale The 2D scale vector.
    virtual void
    setScale(Vec2 scale) noexcept = 0;
    /// @brief Gets scale factors.
    /// @return The 2D scale vector.
    [[nodiscard]] virtual Vec2
    getScale() const noexcept = 0;

    /// @brief Sets object dimensions (width, height).
    /// @param size The 2D size vector.
    virtual void
    setSize(Vec2 size) noexcept = 0;
    /// @brief Gets object dimensions (width, height).
    /// @return The 2D size vector.
    [[nodiscard]] virtual Vec2
    getSize() const noexcept = 0;

    /// @brief Computes or returns the transformation matrix.
    /// @return Calculated 3x3 transformation matrix.
    [[nodiscard]] virtual Mat3
    getTransformMatrix() const = 0;

    /// @brief Sets the layer depth within the render pass.
    /// @param layer Depth layer value.
    virtual void
    setLayer(float layer) noexcept = 0;
    /// @brief Gets the layer depth.
    /// @return Depth layer value.
    [[nodiscard]] virtual float
    getLayer() const noexcept = 0;

    /// @brief Sets the main color tint.
    /// @param color Color RGBA vector.
    virtual void
    setColor(Vec4 color) noexcept = 0;
    /// @brief Gets the main color tint.
    /// @return Color RGBA vector.
    [[nodiscard]] virtual Vec4
    getColor() const noexcept = 0;

    /// @brief Sets the material used for drawing.
    /// @param material Pointer to Material instance.
    virtual void
    setMaterial(Material* material) noexcept = 0;
    /// @brief Gets the material pointer.
    /// @return Pointer to Material instance.
    [[nodiscard]] virtual Material*
    getMaterial() const noexcept = 0;

    /// @brief Sets the render pass layer (e.g. WORLD2D, UI).
    /// @param render_layer Render pass layer enum.
    virtual void
    setRender(RenderLayer render_layer) noexcept;
    /// @brief Gets the render pass layer.
    /// @return Render pass layer enum.
    [[nodiscard]] virtual RenderLayer
    getRender() const noexcept;

    /// @brief Sets the UI layout properties.
    /// @param layout UI layout configuration.
    virtual void
    setUILayout(const UILayout& layout) noexcept;
    /// @brief Gets the UI layout properties.
    /// @return UI layout configuration.
    [[nodiscard]] virtual UILayout
    getUILayout() const noexcept;

    /// @brief Convenience setter for UI anchor.
    /// @param anchor UI anchor point.
    virtual void
    setAnchor(Anchor anchor) noexcept;
    /// @brief Convenience getter for UI anchor.
    /// @return UI anchor point.
    [[nodiscard]] virtual Anchor
    getAnchor() const noexcept;

    /// @brief Convenience setter for UI pivot.
    /// @param pivot UI pivot point.
    virtual void
    setPivot(Pivot pivot) noexcept;
    /// @brief Convenience getter for UI pivot.
    /// @return UI pivot point.
    [[nodiscard]] virtual Pivot
    getPivot() const noexcept;

    /// @brief Convenience setter for UI layout offset.
    /// @param offset UI offset vector.
    virtual void
    setOffset(Vec2 offset) noexcept;
    /// @brief Convenience getter for UI layout offset.
    /// @return UI offset vector.
    [[nodiscard]] virtual Vec2
    getOffset() const noexcept;

    /// @brief Gets calculated global screen or world position.
    /// @param renderer Optional pointer to renderer (required for UI swapchain
    /// resolution).
    /// @return Calculated 2D position.
    [[nodiscard]] virtual Vec2
    getGlobalPosition(const Renderer* renderer = nullptr) const;

    /// @brief Gets bounding box rectangle in global coordinates.
    /// @param renderer Optional pointer to renderer.
    /// @return Bounding rectangle.
    [[nodiscard]] virtual RectShape
    getGlobalBounds(const Renderer* renderer = nullptr) const;

    /// @brief Checks if a point (e.g. mouse cursor) intersects this object.
    /// Transforms point into local element coordinates using matrix inversion.
    /// @param point Point to test (e.g. mouse position).
    /// @param renderer Optional pointer to renderer.
    /// @return True if point is inside object bounds.
    [[nodiscard]] virtual bool
    containsPoint(
        Vec2 point, [[maybe_unused]] const Renderer* renderer = nullptr
    ) const;

protected:
    /// @brief Flag indicating if object is visible.
    bool is_visible = true;
    /// @brief UI layout configuration data.
    UILayout ui_layout;
    /// @brief The render layer
    RenderLayer render_layer = RenderLayer::WORLD2D;
};

}  // namespace lili
