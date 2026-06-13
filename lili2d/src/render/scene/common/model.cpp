#include "render/scene/common/model.hpp"

namespace lili {

Model::Model() {
	mesh = nullptr;
	material = nullptr;
}

Model::Model(GPUMesh *mesh, Material *material)
	: mesh(mesh), material(material) {}

}  // namespace lili
