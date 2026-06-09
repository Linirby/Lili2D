#pragma once

#include <memory>

#include "geometry/vec4.hpp"
#include "geometry/vec2.hpp"
#include "render/renderer.hpp"
#include "render/core/gpu_mesh.hpp"
#include "render/scene/material.hpp"

namespace lili {

/// @brief Defines the geometry of a rectangle.
struct RectShape {
	float x, y, w, h = 0;  ///< Position and dimensions.

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
	void set_position(Vec2 pos);
	/// @brief Sets the rectangle's size.
	/// @param size The new size.
	void set_size(Vec2 size);
	/// @brief Sets the rectangle's rotation.
	/// @param degree The rotation in degrees.
	void set_rotation(float degree);
	/// @brief Sets the rectangle's shape.
	/// @param shape The new shape.
	void set_shape(RectShape shape);
	/// @brief Sets the rectangle's color.
	/// @param color The new color.
	void set_color(Vec4 color);
	/// @brief Sets the depth value for Z-ordering.
	///
	/// This determines the drawing order relative to other objects within the
	/// same render pass.
	/// To change which render pass this object belongs to, use set_render().
	///
	/// @param value The new layer depth.
	void set_layer(float value);
	/// @brief Sets the render pass layer.
	///
	/// This determines which overall pass (e.g., WORLD2D or UI) the object is
	/// drawn in.
	/// To change the depth ordering within a pass, use set_layer().
	///
	/// @param render_layer The new render pass layer.
	void set_render(RenderLayer render_layer);

	/// @brief Gets the position.
	/// @return The position.
	Vec2 get_position() const;
	/// @brief Gets the size.
	/// @return The size.
	Vec2 get_size() const;
	/// @brief Gets the shape geometry.
	/// @return The shape.
	RectShape get_shape() const;
	/// @brief Gets the color.
	/// @return The color.
	Vec4 get_color() const;
	/// @brief Gets the material.
	/// @return Pointer to the material.
	Material* get_material() const;

	/// @brief Submits the rectangle for drawing.
	void draw();

private:
	Renderer *renderer = nullptr;
	RectShape shape;
	float rotation = 0;

	float layer = 0;
	RenderLayer render_layer = RenderLayer::WORLD2D;
	GPUMesh *mesh = nullptr;
	std::unique_ptr<Material> material = nullptr;
};

}  // namespace lili
