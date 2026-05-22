#pragma once

#include "geometry/mat3x3.hpp"
#include "geometry/vec2.hpp"

namespace lili {

class Camera {
public:
	Camera() = default;
	~Camera() = default;

	Vec2 position = { 0.0f, 0.0f };
	float rotation = 0.0f;
	float zoom = 1.0f;

	Mat3 get_view_matrix() const;
	Mat3 get_projection(float width, float height) const;
};

}  // namespace lili
