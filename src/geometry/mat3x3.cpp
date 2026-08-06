#include "lili2d/geometry/mat3x3.hpp"

#include <cmath>

namespace lili {

Mat3
Mat3::operator*(const Mat3& other) const {
    Mat3 result;

    for (int col = 0; col < 3; ++col) {
        for (int row = 0; row < 3; ++row) {
            result.m[col * 3 + row] =
                (m[0 * 3 + row] * other.m[col * 3 + 0] +
                 m[1 * 3 + row] * other.m[col * 3 + 1] +
                 m[2 * 3 + row] * other.m[col * 3 + 2]);
        }
    }
    return result;
}

Vec2
Mat3::transformPoint(const Vec2& point) const {
    float x = m[0] * point.x + m[3] * point.y + m[6];
    float y = m[1] * point.x + m[4] * point.y + m[7];
    float w = m[2] * point.x + m[5] * point.y + m[8];

    if (std::abs(w) > 1e-6f && std::abs(w - 1.0f) > 1e-6f) {
        float inv_w = 1.0f / w;
        return {x * inv_w, y * inv_w};
    }
    return {x, y};
}

Vec2
Mat3::transformVector(const Vec2& vector) const {
    float x = m[0] * vector.x + m[3] * vector.y;
    float y = m[1] * vector.x + m[4] * vector.y;
    return {x, y};
}

float
Mat3::determinant() const {
    return m[0] * (m[4] * m[8] - m[7] * m[5]) -
           m[3] * (m[1] * m[8] - m[7] * m[2]) +
           m[6] * (m[1] * m[5] - m[4] * m[2]);
}

Mat3
Mat3::inverse() const {
    float det = determinant();
    if (std::abs(det) < 1e-8f) {
        return identity();
    }

    float inv_det = 1.0f / det;

    Mat3 result;
    result.m[0] = (m[4] * m[8] - m[7] * m[5]) * inv_det;
    result.m[1] = -(m[1] * m[8] - m[7] * m[2]) * inv_det;
    result.m[2] = (m[1] * m[5] - m[4] * m[2]) * inv_det;

    result.m[3] = -(m[3] * m[8] - m[6] * m[5]) * inv_det;
    result.m[4] = (m[0] * m[8] - m[6] * m[2]) * inv_det;
    result.m[5] = -(m[0] * m[5] - m[3] * m[2]) * inv_det;

    result.m[6] = (m[3] * m[7] - m[6] * m[4]) * inv_det;
    result.m[7] = -(m[0] * m[7] - m[6] * m[1]) * inv_det;
    result.m[8] = (m[0] * m[4] - m[3] * m[1]) * inv_det;

    return result;
}

Mat3
Mat3::transpose() const {
    Mat3 result;
    for (int col = 0; col < 3; ++col) {
        for (int row = 0; row < 3; ++row) {
            result.m[row * 3 + col] = m[col * 3 + row];
        }
    }
    return result;
}

Mat3
Mat3::identity() {
    Mat3 result = {};

    result.m[0] = 1.0f;
    result.m[4] = 1.0f;
    result.m[8] = 1.0f;
    return result;
}

Mat3
Mat3::orthographic(float left, float right, float bottom, float top) {
    Mat3 result = {};

    result.m[0] = 2.0f / (right - left);
    result.m[4] = 2.0f / (bottom - top);
    result.m[6] = -(right + left) / (right - left);
    result.m[7] = -(bottom + top) / (bottom - top);
    result.m[8] = 1.0f;
    return result;
}

Mat3
Mat3::translate(Vec2 pos) {
    Mat3 result = identity();

    result.m[6] = pos.x;
    result.m[7] = pos.y;
    return result;
}

Mat3
Mat3::rotation(float radians) {
    Mat3 result = identity();
    float c = std::cos(radians);
    float s = std::sin(radians);

    result.m[0] = c;
    result.m[1] = s;
    result.m[3] = -s;
    result.m[4] = c;
    return result;
}

Mat3
Mat3::scale(Vec2 v) {
    Mat3 result = identity();

    result.m[0] = v.x;
    result.m[4] = v.y;
    return result;
}

}  // namespace lili
