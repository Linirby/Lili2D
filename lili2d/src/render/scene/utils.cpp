#include "render/scene/utils.hpp"

namespace lili {

MeshData create_unit_quad() {
	MeshData quad;

	quad.vertices = {
		(Vertex){
			.x = -0.5f, .y = -0.5f, .z = 0.0f,
			.u = 0.0f, .v = 0.0f,
			.material_id = 0
		},
		(Vertex){
			.x = 0.5f, .y = -0.5f, .z = 0.0f,
			.u = 1.0f, .v = 0.0f,
			.material_id = 0
		},
		(Vertex){
			.x = 0.5f, .y = 0.5f, .z = 0.0f,
			.u = 1.0f, .v = 1.0f,
			.material_id = 0
		},
		(Vertex){
			.x = -0.5f, .y = 0.5f, .z = 0.0f,
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

}  // namespace lili
