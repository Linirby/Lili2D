#include "point2.hpp"
#include <math.h>

namespace lili {

Point2::Point2(int x, int y) : x(x), y(y) {}

Point2 Point2::operator-(const Point2 &o) const {
	return { x - o.x, y - o.y };
}

Point2 Point2::operator-=(const Point2 &o) {
	x = x - o.x;
	y = y - o.y;
	return *this;
}

Point2 Point2::operator+(const Point2 &o) const {
	return { x + o.x, y + o.y };
}

Point2 Point2::operator+=(const Point2 &o) {
	x = x + o.x;
	y = y + o.y;
	return *this;
}

Point2 Point2::operator*(const int scalar) const {
	return { x * scalar, y * scalar };
}

Point2 Point2::operator*=(const float scalar) {
	x = x * scalar;
	y = y * scalar;
	return *this;
}

}  // namespace lili
