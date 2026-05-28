#include "render/scene/camera.hpp"

namespace lili {

void Camera::set_position(const Vec2 &pos) {
	position = pos;
	is_dirty = true;
}

void Camera::set_rotation(float degree) {
	rotation = degree;
	is_dirty = true;
}

void Camera::set_zoom(float value) {
	zoom = value;
	is_dirty = true;
}

Vec2 Camera::get_position() const {
	return position;
}

float Camera::get_zoom() const {
	return zoom;
}

Mat3 Camera::get_view_matrix(float width, float height) const {
	if (is_dirty) {
		Mat3 translate = Mat3::translate({ -position.x, -position.y });
		Mat3 rotate = Mat3::rotation(-rotation);
		Mat3 scale = Mat3::scale({ zoom, zoom });
		Mat3 center = Mat3::translate({ width / 2.0f, height / 2.0f });

		cached_view_matrix = center * scale * rotate * translate;
		is_dirty = false;
	}
	return cached_view_matrix;
}

Mat3 Camera::get_projection(float width, float height) const {
	return Mat3::orthographic(0.0f, width, 0.0f, height);
}

}  // namespace lili
