#include "lili2d/geometry/point3.hpp"
namespace lili {

Point3::Point3(int x, int y, int z) : x(x), y(y), z(z) {}

Point3 Point3::operator-(const Point3 &o) const {
	return { x - o.x, y - o.y, z - o.z };
}

Point3 Point3::operator-=(const Point3 &o) {
	x = x - o.x;
	y = y - o.y;
	z = z - o.z;
	return *this;
}

Point3 Point3::operator+(const Point3 &o) const {
	return { x + o.x, y + o.y, z + o.z };
}

Point3 Point3::operator+=(const Point3 &o) {
	x = x + o.x;
	y = y + o.y;
	z = z + o.z;
	return *this;
}

Point3 Point3::operator*(const int scalar) const {
	return { x * scalar, y * scalar, z * scalar };
}

Point3 Point3::operator*=(const float scalar) {
	x = x * scalar;
	y = y * scalar;
	z = z * scalar;
	return *this;
}

}  // namespace lili
