#pragma once

#include "geometry/mat3x3.hpp"
#include "geometry/vec2.hpp"

namespace lili {

class Camera {
public:
	Camera() = default;
	~Camera() = default;

	void set_position(const Vec2 &pos);
	void set_rotation(float degree);

	Vec2 get_position() const;
	Mat3 get_view_matrix() const;
	Mat3 get_projection(float width, float height) const;

private:
	Vec2 position = { 0.0f, 0.0f };
	float rotation = 0.0f;
	float zoom = 1.0f;
};

}  // namespace lili
