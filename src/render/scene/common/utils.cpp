#include "lili2d/render/scene/common/utils.hpp"
#include <cmath>

namespace lili {

MeshData createUnitQuad() {
	MeshData quad;

	quad.vertices = {
		(Vertex){
			.x = 0.0f, .y = 0.0f, .z = 0.0f,
			.u = 0.0f, .v = 0.0f,
			.material_id = 0
		},
		(Vertex){
			.x = 1.0f, .y = 0.0f, .z = 0.0f,
			.u = 1.0f, .v = 0.0f,
			.material_id = 0
		},
		(Vertex){
			.x = 1.0f, .y = 1.0f, .z = 0.0f,
			.u = 1.0f, .v = 1.0f,
			.material_id = 0
		},
		(Vertex){
			.x = 0.0f, .y = 1.0f, .z = 0.0f,
			.u = 0.0f, .v = 1.0f,
			.material_id = 0
		}
	};
	quad.indices = {
		0, 1, 2,
		2, 3, 0
	};
	return quad;
}

MeshData createUnitCircle(int segments) {
	MeshData circle;

	if (segments < 3) segments = 3;

	const float PI = 3.14159265359f;
	float angle_step = (2.0f * PI) / segments;

	circle.vertices.push_back((Vertex){
		.x = 0.0f, .y = 0.0f, .z = 0.0f,
		.u = 0.5f, .v = 0.5f,
		.material_id = 0
	});

	for (int i = 0; i < segments; i++) {
		float angle = i * angle_step;

		float x = std::cos(angle) * 0.5f;
		float y = std::sin(angle) * 0.5f;

		float u = x + 0.5f;
		float v = y + 0.5f;

		circle.vertices.push_back((Vertex){
			.x = x, .y = y, .z = 0.0f,
			.u = u, .v = v,
			.material_id = 0
		});
	}

	for (int i = 1; i <= segments; i++) {
		uint32_t center_index = 0;
		uint32_t current_index = i;
		uint32_t next_index = (i == segments) ? 1 : (i + 1);

		circle.indices.push_back(center_index);
		circle.indices.push_back(current_index);
		circle.indices.push_back(next_index);
	}

	return circle;
}

}  // namespace lili
