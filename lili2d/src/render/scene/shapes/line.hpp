#pragma once

#include <memory>

#include "geometry/vec4.hpp"
#include "geometry/vec2.hpp"
#include "render/renderer.hpp"
#include "render/core/gpu_mesh.hpp"
#include "render/scene/material.hpp"

namespace lili {

/**
 * \brief Defines the geometry of a line.
 */
struct LineShape {
	Vec2 start;      ///< The start position.
	Vec2 end;        ///< The end position.
	float thickness; ///< The thickness of the line.

	/// \brief Default constructor.
	LineShape() = default;
	/// \brief Copy constructor.
	LineShape(const LineShape &) = default;
	/**
	 * \brief Constructs a line shape.
	 * \param start The start position.
	 * \param end The end position.
	 * \param thickness The thickness.
	 */
	LineShape(Vec2 start, Vec2 end, float thickness);
};

/**
 * \brief A renderable line.
 */
class Line {
public:
	/// \brief Default constructor.
	Line() = default;
	/**
	 * \brief Constructs a renderable line.
	 * \param renderer The renderer.
	 * \param shape The line geometry.
	 * \param color The color.
	 */
	Line(Renderer *renderer, LineShape shape, Vec4 color);
	/// \brief Default destructor.
	~Line() = default;

	/// \brief Move constructor.
	Line(Line&&) = default;
	/**
	 * \brief Move assignment operator.
	 * \return Reference to the assigned line.
	 */
	Line& operator=(Line&&) = default;

	/**
	 * \brief Sets the start position.
	 * \param pos The new start position.
	 */
	void set_start(Vec2 pos);
	/**
	 * \brief Sets the end position.
	 * \param pos The new end position.
	 */
	void set_end(Vec2 pos);
	/**
	 * \brief Sets the thickness.
	 * \param value The new thickness.
	 */
	void set_thickness(float value);
	/**
	 * \brief Sets the line shape.
	 * \param shape The new shape.
	 */
	void set_shape(LineShape shape);
	/**
	 * \brief Sets the color.
	 * \param color The new color.
	 */
	void set_color(Vec4 color);
	/**
	 * \brief Sets the rendering layer.
	 * \param value The new layer depth.
	 */
	void set_layer(float value);

	/**
	 * \brief Gets the start position.
	 * \return The start position.
	 */
	Vec2 get_start() const;
	/**
	 * \brief Gets the end position.
	 * \return The end position.
	 */
	Vec2 get_end() const;
	/**
	 * \brief Gets the thickness.
	 * \return The thickness.
	 */
	Vec2 get_thickness() const;
	/**
	 * \brief Gets the shape geometry.
	 * \return The shape.
	 */
	LineShape get_shape() const;
	/**
	 * \brief Gets the color.
	 * \return The color.
	 */
	Vec4 get_color() const;

	/// \brief Submits the line for drawing.
	void draw();

private:
	Renderer *renderer = nullptr;
	LineShape shape{};

	float layer = 0.0f;
	std::unique_ptr<GPUMesh> mesh;
	std::unique_ptr<Material> material;
};

}  // namespace lili
