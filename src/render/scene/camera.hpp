#pragma once

#include "geometry/vec3.hpp"
#include "geometry/vec4.hpp"
#include "geometry/mat4x4.hpp"
#include "physics/collision.hpp"

namespace lili {

struct Frustum {
	Vec4 planes[6];

	bool contains(const AABB &box) const;
};

class Camera {
public:
	Camera();
	~Camera() = default;

	Vec3 position = { 0.0f, 0.0f, 0.0f };
	Vec3 front = { 0.0f, 0.0f, -1.0f };
	Vec3 right = { 1.0f, 0.0f,  0.0f };
	Vec3 up    = { 0.0f, 1.0f,  0.0f };

	float yaw = -90.0f;
	float pitch = 0.0f;
	float mouse_sensitivity = 0.1f;

	float fov_y = 90.0f;
	float near_dist = 0.3f;
	float far_dist = 100.0f;

	void set_rotation(float new_yaw, float new_pitch);
	void set_fov(float degree);
	void process_mouse(float offset_x, float offset_y);
	Mat4 get_view_matrix() const;
	Mat4 get_projection(float aspect_ratio) const;
	Frustum get_frustum(float aspect_ratio) const;

private:
	void update_vectors();
};

}  // namespace lili
