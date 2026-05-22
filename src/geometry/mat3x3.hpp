#pragma once

#include "geometry/vec2.hpp"

namespace lili {

struct Mat3 {
	float m[9] = { 0.0f };

	Mat3 operator*(const Mat3 &other) const;

	static Mat3 identity();
	static Mat3 orthographic(
		float left, float right, float bottom, float top
	);
	static Mat3 translate(const Vec2 &pos);
	static Mat3 rotation(float radians);
	static Mat3 scale(const Vec2 &v);
};

}  // namespace lili
