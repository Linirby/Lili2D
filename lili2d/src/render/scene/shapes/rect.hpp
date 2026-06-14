#pragma once

#include <memory>

#include "../../../geometry/vec4.hpp"
#include "../../../geometry/vec2.hpp"
#include "../../renderer.hpp"
#include "../../core/gpu_mesh.hpp"
#include "../common/material.hpp"

namespace lili {

/// @brief Defines the geometry of a rectangle.
struct RectShape {
	float x = 0; ///< X position.
	float y = 0; ///< Y position.
	float w = 0; ///< Width.
	float h = 0; ///< Height.

	/// @brief Default constructor.
	RectShape() = default;
	/// @brief Copy constructor.
	RectShape(const RectShape &) = default;
	/// @brief Constructs a rectangle shape.
	/// @param x X coordinate.
	/// @param y Y coordinate.
	/// @param w Width.
	/// @param h Height.
	RectShape(float x, float y, float w, float h);
};

/// @brief A renderable rectangle.
class Rect {
public:
	/// @brief Default constructor.
	Rect() = default;
	/// @brief Constructs a renderable rectangle.
	/// @param renderer The renderer.
	/// @param shape The rectangle geometry.
	/// @param color The color of the rectangle.
	Rect(Renderer *renderer, RectShape shape, Vec4 color);
	/// @brief Default destructor.


	/// @brief Move constructor.
	Rect(Rect&&) = default;
	/// @brief Move assignment operator.
	/// @return Reference to the assigned rectangle.
	Rect& operator=(Rect&&) = default;

	/// @brief Sets the rectangle's position.
	/// @param pos The new position.
	void setPosition(Vec2 pos);
	/// @brief Sets the rectangle's size.
	/// @param size The new size.
	void setSize(Vec2 size);
	/// @brief Sets the rectangle's rotation.
	/// @param degree The rotation in degrees.
	void setRotation(float degree);
	/// @brief Sets the rectangle's shape.
	/// @param shape The new shape.
	void setShape(RectShape shape);
	/// @brief Sets the rectangle's color.
	/// @param color The new color.
	void setColor(Vec4 color);
	/// @brief Sets whether the rectangle is hollow.
	/// @param hollow The new hollow state.
	void setHollow(bool hollow);
	/// @brief Sets the outline thickness when hollow.
	/// @param thickness The thickness of the outline.
	void setHollowThickness(float thickness);
	/// @brief Sets the depth value for Z-ordering.
	///
	/// This determines the drawing order relative to other objects within the
	/// same render pass.
	/// To change which render pass this object belongs to, use setRender().
	///
	/// @param value The new layer depth.
	void setLayer(float value);
	/// @brief Sets the render pass layer.
	///
	/// This determines which overall pass (e.g., WORLD2D or UI) the object is
	/// drawn in.
	/// To change the depth ordering within a pass, use setLayer().
	///
	/// @param render_layer The new render pass layer.
	void setRender(RenderLayer render_layer);

	/// @brief Gets the position.
	/// @return The position.
	Vec2 getPosition() const;
	/// @brief Gets the size.
	/// @return The size.
	Vec2 getSize() const;
	/// @brief Gets the shape geometry.
	/// @return The shape.
	RectShape getShape() const;
	/// @brief Gets the color.
	/// @return The color.
	Vec4 getColor() const;
	/// @brief Gets the material.
	/// @return Pointer to the material.
	Material* getMaterial() const;
	/// @brief Returns whether the rectangle is hollow.
	/// @return True if hollow, false otherwise.
	bool isHollow() const;
	/// @brief Gets the hollow thickness.
	/// @return The hollow outline thickness.
	float getHollowThickness() const;

	/// @brief Submits the rectangle for drawing.
	void draw();

private:
	Renderer *renderer = nullptr;
	RectShape shape;
	float rotation = 0;
	bool is_hollow = false;
	float hollow_thickness = 1.0f;

	float layer = 0;
	RenderLayer render_layer = RenderLayer::WORLD2D;
	GPUMesh *mesh = nullptr;
	std::unique_ptr<Material> material = nullptr;
};

}  // namespace lili
