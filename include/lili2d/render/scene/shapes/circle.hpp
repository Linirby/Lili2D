#pragma once

#include <memory>

#include "lili2d/geometry/vec4.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/renderer.hpp"
#include "lili2d/render/core/gpu_mesh.hpp"
#include "lili2d/render/scene/common/material.hpp"

namespace lili {

/// @brief Defines the geometry of a circle.
struct CircleShape {
	Vec2 center;       ///< The center position.
	float radius = 0;  ///< The radius.
	int segments = 3;  ///< The number of segments.

	/// @brief Default constructor.
	CircleShape() = default;
	/// @brief Copy constructor.
	CircleShape(const CircleShape &) = default;
	/// @brief Constructs a circle shape.
	/// @param center The center position.
	/// @param radius The radius.
	/// @param segments The number of segments.
	CircleShape(Vec2 center, float radius, int segments);
};

/// @brief A renderable circle.
class Circle {
public:
	/// @brief Default constructor.
	Circle() = default;
	/// @brief Constructs a renderable circle.
	/// @param renderer The renderer.
	/// @param shape The circle geometry.
	/// @param color The color.
	Circle(Renderer *renderer, CircleShape shape, Vec4 color);
	/// @brief Default destructor.


	/// @brief Move constructor.
	Circle(Circle&&) = default;
	/// @brief Move assignment operator.
	/// @return Reference to the assigned circle.
	Circle& operator=(Circle&&) = default;

	/// @brief Sets the center position.
	/// @param pos The new center.
	void setCenter(Vec2 pos);
	/// @brief Sets the radius.
	/// @param r The new radius.
	void setRadius(float r);
	/// @brief Sets the number of segments.
	/// @param n The new number of segments.
	void setSegments(int n);
	/// @brief Sets the circle shape.
	/// @param shape The new shape.
	void setShape(CircleShape shape);
	/// @brief Sets the color.
	/// @param color The new color.
	void setColor(Vec4 color);
	/// @brief Sets whether the circle is hollow.
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

	/// @brief Gets the center position.
	/// @return The center position.
	Vec2 getCenter() const;
	Vec2 getTopLeft() const;
	/// @brief Gets the radius.
	/// @return The radius.
	float getRadius() const;
	float getDiameter() const;
	/// @brief Gets the shape geometry.
	/// @return The shape.
	CircleShape getShape() const;
	/// @brief Gets the color.
	/// @return The color.
	Vec4 getColor() const;
	/// @brief Gets the material.
	/// @return Pointer to the material.
	Material* getMaterial() const;
	/// @brief Returns whether the circle is hollow.
	/// @return True if hollow, false otherwise.
	bool isHollow() const;
	/// @brief Gets the hollow thickness.
	/// @return The hollow outline thickness.
	float getHollowThickness() const;

	/// @brief Submits the circle for drawing.
	void draw();

private:
	Renderer *renderer = nullptr;
	CircleShape shape;
	bool is_hollow = false;
	float hollow_thickness = 1.0f;

	float layer = 0;
	RenderLayer render_layer = RenderLayer::WORLD2D;
	GPUMesh *mesh = nullptr;
	std::unique_ptr<Material> material = nullptr;
};

}  // namespace lili
