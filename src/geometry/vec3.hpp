#pragma once

namespace lili {

struct Vec3 {
	float x, y, z;

	Vec3(float x, float y, float z);
	Vec3(const Vec3 &);

	Vec3 operator=(const Vec3 &o);
	Vec3 operator-(const Vec3 &o) const;
	Vec3 operator-=(const Vec3 &o);
	Vec3 operator+(const Vec3 &o) const;
	Vec3 operator+=(const Vec3 &o);
	Vec3 operator*(const float scalar) const;
	Vec3 operator*=(const float scalar);

	float dot(const Vec3 &o) const;
	Vec3 cross(const Vec3 &o) const;
	Vec3 normalized() const;
	Vec3 lerp(const Vec3 &o, float t) const;
	float length() const;
};

}  // namespace lili
