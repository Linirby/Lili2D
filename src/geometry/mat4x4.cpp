#include "lili2d/geometry/mat4x4.hpp"

#include <cmath>

namespace lili {

Mat4
Mat4::operator*(const Mat4& other) const {
    Mat4 result;

    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            result.m[col * 4 + row] =
                (m[0 * 4 + row] * other.m[col * 4 + 0] +
                 m[1 * 4 + row] * other.m[col * 4 + 1] +
                 m[2 * 4 + row] * other.m[col * 4 + 2] +
                 m[3 * 4 + row] * other.m[col * 4 + 3]);
        }
    }
    return result;
}

Vec3
Mat4::transformPoint(const Vec3& point) const {
    float x = m[0] * point.x + m[4] * point.y + m[8] * point.z + m[12];
    float y = m[1] * point.x + m[5] * point.y + m[9] * point.z + m[13];
    float z = m[2] * point.x + m[6] * point.y + m[10] * point.z + m[14];
    float w = m[3] * point.x + m[7] * point.y + m[11] * point.z + m[15];

    if (std::abs(w) > 1e-6f && std::abs(w - 1.0f) > 1e-6f) {
        float inv_w = 1.0f / w;
        return {x * inv_w, y * inv_w, z * inv_w};
    }
    return {x, y, z};
}

Vec3
Mat4::transformVector(const Vec3& vector) const {
    float x = m[0] * vector.x + m[4] * vector.y + m[8] * vector.z;
    float y = m[1] * vector.x + m[5] * vector.y + m[9] * vector.z;
    float z = m[2] * vector.x + m[6] * vector.y + m[10] * vector.z;
    return {x, y, z};
}

float
Mat4::determinant() const {
    float s0 = m[0] * m[5] - m[4] * m[1];
    float s1 = m[0] * m[9] - m[8] * m[1];
    float s2 = m[0] * m[13] - m[12] * m[1];
    float s3 = m[4] * m[9] - m[8] * m[5];
    float s4 = m[4] * m[13] - m[12] * m[5];
    float s5 = m[8] * m[13] - m[12] * m[9];

    float c5 = m[10] * m[15] - m[14] * m[11];
    float c4 = m[6] * m[15] - m[14] * m[7];
    float c3 = m[6] * m[11] - m[10] * m[7];
    float c2 = m[2] * m[15] - m[14] * m[3];
    float c1 = m[2] * m[11] - m[10] * m[3];
    float c0 = m[2] * m[7] - m[6] * m[3];

    return s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
}

Mat4
Mat4::inverse() const {
    float s0 = m[0] * m[5] - m[4] * m[1];
    float s1 = m[0] * m[9] - m[8] * m[1];
    float s2 = m[0] * m[13] - m[12] * m[1];
    float s3 = m[4] * m[9] - m[8] * m[5];
    float s4 = m[4] * m[13] - m[12] * m[5];
    float s5 = m[8] * m[13] - m[12] * m[9];

    float c5 = m[10] * m[15] - m[14] * m[11];
    float c4 = m[6] * m[15] - m[14] * m[7];
    float c3 = m[6] * m[11] - m[10] * m[7];
    float c2 = m[2] * m[15] - m[14] * m[3];
    float c1 = m[2] * m[11] - m[10] * m[3];
    float c0 = m[2] * m[7] - m[6] * m[3];

    float det = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
    if (std::abs(det) < 1e-8f) {
        return identity();
    }

    float inv_det = 1.0f / det;

    Mat4 inv;
    inv.m[0] = (m[5] * c5 - m[9] * c4 + m[13] * c3) * inv_det;
    inv.m[1] = (-m[1] * c5 + m[9] * c2 - m[13] * c1) * inv_det;
    inv.m[2] = (m[1] * c4 - m[5] * c2 + m[13] * c0) * inv_det;
    inv.m[3] = (-m[1] * c3 + m[5] * c1 - m[9] * c0) * inv_det;

    inv.m[4] = (-m[4] * c5 + m[8] * c4 - m[12] * c3) * inv_det;
    inv.m[5] = (m[0] * c5 - m[8] * c2 + m[12] * c1) * inv_det;
    inv.m[6] = (-m[0] * c4 + m[4] * c2 - m[12] * c0) * inv_det;
    inv.m[7] = (m[0] * c3 - m[4] * c1 + m[8] * c0) * inv_det;

    inv.m[8] = (m[7] * s5 - m[11] * s4 + m[15] * s3) * inv_det;
    inv.m[9] = (-m[3] * s5 + m[11] * s2 - m[15] * s1) * inv_det;
    inv.m[10] = (m[3] * s4 - m[7] * s2 + m[15] * s0) * inv_det;
    inv.m[11] = (-m[3] * s3 + m[7] * s1 - m[11] * s0) * inv_det;

    inv.m[12] = (-m[6] * s5 + m[10] * s4 - m[14] * s3) * inv_det;
    inv.m[13] = (m[2] * s5 - m[10] * s2 + m[14] * s1) * inv_det;
    inv.m[14] = (-m[2] * s4 + m[6] * s2 - m[14] * s0) * inv_det;
    inv.m[15] = (m[2] * s3 - m[6] * s1 + m[10] * s0) * inv_det;

    return inv;
}

Mat4
Mat4::transpose() const {
    Mat4 result;
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            result.m[row * 4 + col] = m[col * 4 + row];
        }
    }
    return result;
}


Mat4
Mat4::identity() {
    Mat4 result = {};

    result.m[0] = 1.0f;
    result.m[5] = 1.0f;
    result.m[10] = 1.0f;
    result.m[15] = 1.0f;
    return result;
}

Mat4
Mat4::lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
    Vec3 f = (center - eye).normalized();
    Vec3 s = f.cross(up).normalized();
    Vec3 u = s.cross(f);
    Mat4 result = identity();

    // X axis
    result.m[0] = s.x;
    result.m[1] = u.x;
    result.m[2] = -f.x;
    // Y axis
    result.m[4] = s.y;
    result.m[5] = u.y;
    result.m[6] = -f.y;
    // Z axis
    result.m[8] = s.z;
    result.m[9] = u.z;
    result.m[10] = -f.z;
    // Camera translation
    result.m[12] = -s.dot(eye);
    result.m[13] = -u.dot(eye);
    result.m[14] = f.dot(eye);
    result.m[15] = 1.0f;
    return result;
}

Mat4
Mat4::perspective(float fovy_rad, float aspect, float near, float far) {
    Mat4 result = {};

    float tan_half_fovy = std::tan(fovy_rad * 0.5f);
    result.m[0] = 1.0f / (aspect * tan_half_fovy);
    result.m[5] = 1.0f / tan_half_fovy;
    result.m[10] = -far / (far - near);
    result.m[11] = -1.0f;
    result.m[14] = -(far * near) / (far - near);
    return result;
}

Mat4
Mat4::orthographic(
    float left, float right, float bottom, float top, float near, float far
) {
    Mat4 result = {};

    result.m[0] = 2.0f / (right - left);
    result.m[5] = 2.0f / (bottom - top);
    result.m[10] = -1.0f / (far - near);
    result.m[12] = -(right + left) / (right - left);
    result.m[13] = -(bottom + top) / (bottom - top);
    result.m[14] = -near / (far - near);
    result.m[15] = 1.0f;
    return result;
}

Mat4
Mat4::translate(Vec3 pos) {
    Mat4 result = identity();

    result.m[12] = pos.x;
    result.m[13] = pos.y;
    result.m[14] = pos.z;
    return result;
}

Mat4
Mat4::rotationX(float radian) {
    Mat4 result = identity();
    float c = std::cos(radian);
    float s = std::sin(radian);

    result.m[5] = c;
    result.m[6] = s;
    result.m[9] = -s;
    result.m[10] = c;
    return result;
}

Mat4
Mat4::rotationY(float radian) {
    Mat4 result = identity();
    float c = std::cos(radian);
    float s = std::sin(radian);

    result.m[0] = c;
    result.m[2] = -s;
    result.m[8] = s;
    result.m[10] = c;
    return result;
}

Mat4
Mat4::rotationZ(float radian) {
    Mat4 result = identity();
    float c = std::cos(radian);
    float s = std::sin(radian);

    result = Mat4::identity();
    result.m[0] = c;
    result.m[1] = s;
    result.m[4] = -s;
    result.m[5] = c;
    return result;
}

Mat4
Mat4::rotationXyz(Vec3 radians) {
    return (rotationX(radians.x) * rotationY(radians.y) * rotationZ(radians.z));
}

Mat4
Mat4::scale(Vec3 v) {
    Mat4 result = identity();

    result.m[0] = v.x;
    result.m[5] = v.y;
    result.m[10] = v.z;
    return result;
}

}  // namespace lili
