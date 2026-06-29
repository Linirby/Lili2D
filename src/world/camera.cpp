#include "lili2d/world/camera.hpp"
#include <cmath>

namespace lili {

void Camera::setPosition(const Vec2 &pos) {
	position = pos;
	is_dirty = true;
}

void Camera::setRotation(float degree) {
	rotation = degree;
	is_dirty = true;
}

void Camera::setZoom(float value) {
	zoom = value;
	is_dirty = true;
}

Vec2 Camera::getPosition() const {
	return position;
}

float Camera::getZoom() const {
	return zoom;
}

Mat3 Camera::getViewMatrix(float width, float height) const {
	if (is_dirty) {
		float px = std::round(-position.x * zoom) / zoom;
		float py = std::round(-position.y * zoom) / zoom;
		Mat3 translate = Mat3::translate({ px, py });
		Mat3 rotate = Mat3::rotation(-rotation);
		Mat3 scale = Mat3::scale({ zoom, zoom });
		Mat3 center = Mat3::translate({
			std::round(width / 2.0f), std::round(height / 2.0f)
		});

		cached_view_matrix = center * scale * rotate * translate;
		is_dirty = false;
	}
	return cached_view_matrix;
}

Mat3 Camera::getProjection(float width, float height) const {
	return Mat3::orthographic(0.0f, width, 0.0f, height);
}

}  // namespace lili
