#pragma once

#include "geometry/mat3x3.hpp"
#include "geometry/vec2.hpp"

namespace lili {

/**
 * \brief Represents a 2D camera for scene viewing.
 */
class Camera {
public:
	/// \brief Default constructor.
	Camera() = default;
	/// \brief Default destructor.
	~Camera() = default;

	/**
	 * \brief Sets the camera's position.
	 * \param pos The new position.
	 */
	void set_position(const Vec2 &pos);
	/**
	 * \brief Sets the camera's rotation.
	 * \param degree The rotation in degrees.
	 */
	void set_rotation(float degree);
	/**
	 * \brief Sets the camera's zoom level.
	 * \param value The zoom level.
	 */
	void set_zoom(float value);

	/**
	 * \brief Gets the camera's position.
	 * \return The current position.
	 */
	Vec2 get_position() const;
	/**
	 * \brief Gets the camera's zoom level.
	 * \return The current zoom level.
	 */
	float get_zoom() const;
	/**
	 * \brief Gets the view matrix.
	 * \param width The viewport width.
	 * \param height The viewport height.
	 * \return The view transformation matrix.
	 */
	Mat3 get_view_matrix(float width, float height) const;
	/**
	 * \brief Gets the projection matrix.
	 * \param width The viewport width.
	 * \param height The viewport height.
	 * \return The projection matrix.
	 */
	Mat3 get_projection(float width, float height) const;

private:
	Vec2 position;
	float rotation = 0.0f;
	float zoom = 1.0f;
};

}  // namespace lili
