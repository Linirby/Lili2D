#include "render/scene/camera.hpp"

namespace lili {

void Camera::set_position(const Vec2 &pos) {
	position = pos;
}

void Camera::set_rotation(float degree) {
	rotation = degree;
}

Vec2 Camera::get_position() const {
	return position;
}

Mat3 Camera::get_view_matrix() const {
	Mat3 translate = Mat3::translate({ -position.x, -position.y });
	Mat3 rotate = Mat3::rotation(-rotation);
	Mat3 scale = Mat3::scale({ zoom, zoom });

	return scale * rotate * translate;
}

Mat3 Camera::get_projection(float width, float height) const {
	return Mat3::orthographic(0.0f, width, 0.0f, height);
}

}  // namespace lili
