#pragma once

#include <memory>

#include "geometry/vec4.hpp"
#include "geometry/vec2.hpp"
#include "render/renderer.hpp"
#include "render/core/gpu_mesh.hpp"
#include "render/scene/material.hpp"

namespace lili {

/// @brief Defines the geometry of a line.
struct LineShape {
	Vec2 start;             ///< The start position.
	Vec2 end;               ///< The end position.
	float thickness = 0.0;  ///< The thickness of the line.

	/// @brief Default constructor.
	LineShape() = default;
	/// @brief Copy constructor.
	LineShape(const LineShape &) = default;
	/// @brief Constructs a line shape.
	/// @param start The start position.
	/// @param end The end position.
	/// @param thickness The thickness.
	LineShape(Vec2 start, Vec2 end, float thickness);
};

/// @brief A renderable line.
class Line {
public:
	/// @brief Default constructor.
	Line() = default;
	/// @brief Constructs a renderable line.
	/// @param renderer The renderer.
	/// @param shape The line geometry.
	/// @param color The color.
	Line(Renderer *renderer, LineShape shape, Vec4 color);
	/// @brief Default destructor.


	/// @brief Move constructor.
	Line(Line&&) = default;
	/// @brief Move assignment operator.
	/// @return Reference to the assigned line.
	Line& operator=(Line&&) = default;

	/// @brief Sets the start position.
	/// @param pos The new start position.
	void setStart(Vec2 pos);
	/// @brief Sets the end position.
	/// @param pos The new end position.
	void setEnd(Vec2 pos);
	/// @brief Sets the thickness.
	/// @param value The new thickness.
	void setThickness(float value);
	/// @brief Sets the line shape.
	/// @param shape The new shape.
	void setShape(LineShape shape);
	/// @brief Sets the color.
	/// @param color The new color.
	void setColor(Vec4 color);
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

	/// @brief Gets the start position.
	/// @return The start position.
	Vec2 getStart() const;
	/// @brief Gets the end position.
	/// @return The end position.
	Vec2 getEnd() const;
	/// @brief Gets the thickness.
	/// @return The thickness.
	Vec2 getThickness() const;
	/// @brief Gets the shape geometry.
	/// @return The shape.
	LineShape getShape() const;
	/// @brief Gets the color.
	/// @return The color.
	Vec4 getColor() const;
	/// @brief Gets the material.
	/// @return Pointer to the material.
	Material* getMaterial() const;

	/// @brief Submits the line for drawing.
	void draw();

private:
	Renderer *renderer = nullptr;
	LineShape shape;

	float layer = 0;
	RenderLayer render_layer = RenderLayer::WORLD2D;
	GPUMesh *mesh = nullptr;
	std::unique_ptr<Material> material = nullptr;
};

}  // namespace lili
