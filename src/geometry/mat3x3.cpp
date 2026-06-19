#include <cmath>

#include "lili2d/geometry/mat3x3.hpp"

namespace lili {

Mat3 Mat3::operator*(const Mat3 &other) const {
	Mat3 result;

	for (int col = 0; col < 3; ++col) {
		for (int row = 0; row < 3; ++row) {
			result.m[col * 3 + row] = (
				m[0 * 3 + row] * other.m[col * 3 + 0] +
				m[1 * 3 + row] * other.m[col * 3 + 1] +
				m[2 * 3 + row] * other.m[col * 3 + 2]
			);
		}
	}
	return result;
}

Mat3 Mat3::identity() {
	Mat3 result = {};

	result.m[0] = 1.0f;
	result.m[4] = 1.0f;
	result.m[8] = 1.0f;
	return result;
}

Mat3 Mat3::orthographic(
	float left, float right, float bottom, float top
) {
	Mat3 result = {};

	result.m[0] = 2.0f / (right - left);
	result.m[4] = 2.0f / (bottom - top);
	result.m[6] = -(right + left) / (right - left);
	result.m[7] = -(bottom + top) / (bottom - top);
	result.m[8] = 1.0f;
	return result;
}

Mat3 Mat3::translate(Vec2 pos) {
	Mat3 result = identity();

	result.m[6] = pos.x;
	result.m[7] = pos.y;
	return result;
}

Mat3 Mat3::rotation(float radians) {
	Mat3 result = identity();
	float c = std::cos(radians);
	float s = std::sin(radians);

	result.m[0] = c;
	result.m[1] = s;
	result.m[3] = -s;
	result.m[4] = c;
	return result;
}

Mat3 Mat3::scale(Vec2 v) {
	Mat3 result = identity();

	result.m[0] = v.x;
	result.m[4] = v.y;
	return result;
}

}  // namespace lili
