#pragma once

#include <memory>

#include "geometry/vec4.hpp"
#include "geometry/vec2.hpp"
#include "render/renderer.hpp"
#include "render/core/gpu_mesh.hpp"
#include "render/scene/material.hpp"

namespace lili {

/**
 * \brief Defines the geometry of a circle.
 */
struct CircleShape {
	Vec2 center;  ///< The center position.
	float radius = 0; ///< The radius.
	int segments = 3; ///< The number of segments.

	/// \brief Default constructor.
	CircleShape() = default;
	/// \brief Copy constructor.
	CircleShape(const CircleShape &) = default;
	/**
	 * \brief Constructs a circle shape.
	 * \param center The center position.
	 * \param radius The radius.
	 * \param segments The number of segments.
	 */
	CircleShape(Vec2 center, float radius, int segments);
};

/**
 * \brief A renderable circle.
 */
class Circle {
public:
	/// \brief Default constructor.
	Circle() = default;
	/**
	 * \brief Constructs a renderable circle.
	 * \param renderer The renderer.
	 * \param shape The circle geometry.
	 * \param color The color.
	 */
	Circle(Renderer *renderer, CircleShape shape, Vec4 color);
	/// \brief Default destructor.
	~Circle() = default;

	/// \brief Move constructor.
	Circle(Circle&&) = default;
	/**
	 * \brief Move assignment operator.
	 * \return Reference to the assigned circle.
	 */
	Circle& operator=(Circle&&) = default;

	/**
	 * \brief Sets the center position.
	 * \param pos The new center.
	 */
	void set_center(Vec2 pos);
	/**
	 * \brief Sets the radius.
	 * \param r The new radius.
	 */
	void set_radius(float r);
	/**
	 * \brief Sets the number of segments.
	 * \param n The new number of segments.
	 */
	void set_segments(int n);
	/**
	 * \brief Sets the circle shape.
	 * \param shape The new shape.
	 */
	void set_shape(CircleShape shape);
	/**
	 * \brief Sets the color.
	 * \param color The new color.
	 */
	void set_color(Vec4 color);
	/**
	 * \brief Sets the depth value for Z-ordering.
	 * 
	 * This determines the drawing order relative to other objects within the same render pass.
	 * To change which render pass this object belongs to, use set_render().
	 * 
	 * \param value The new layer depth.
	 */
	void set_layer(float value);
	/**
	 * \brief Sets the render pass layer.
	 * 
	 * This determines which overall pass (e.g., WORLD2D or UI) the object is drawn in.
	 * To change the depth ordering within a pass, use set_layer().
	 * 
	 * \param render_layer The new render pass layer.
	 */
	void set_render(RenderLayer render_layer);

	/**
	 * \brief Gets the center position.
	 * \return The center position.
	 */
	Vec2 get_center() const;
	/**
	 * \brief Gets the radius.
	 * \return The radius.
	 */
	float get_radius() const;
	/**
	 * \brief Gets the shape geometry.
	 * \return The shape.
	 */
	CircleShape get_shape() const;
	/**
	 * \brief Gets the color.
	 * \return The color.
	 */
	Vec4 get_color() const;
	/**
	 * \brief Gets the material.
	 * \return Pointer to the material.
	 */
	Material* get_material() const;

	/// \brief Submits the circle for drawing.
	void draw();

private:
	Renderer *renderer = nullptr;
	CircleShape shape;

	float layer = 0;
	RenderLayer render_layer = RenderLayer::WORLD2D;
	GPUMesh *mesh = nullptr;
	std::unique_ptr<Material> material = nullptr;
};

}  // namespace lili
