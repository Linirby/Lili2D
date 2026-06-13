#include "render/scene/common/material.hpp"

#include <stdexcept>

namespace lili {

Material::Material(Texture *texture)
	: albedoMap(texture), custom_pipeline(nullptr) {}

Material::Material(Texture *texture, SDL_GPUGraphicsPipeline *pipeline)
	: albedoMap(texture), custom_pipeline(pipeline) {}

MaterialRegistry &MaterialRegistry::get() {
	static MaterialRegistry instance;
	return instance;
}

uint16_t MaterialRegistry::registerMaterial(
	const std::string &key, const Material &material
) {
	auto it = key_to_id.find(key);
	if (it != key_to_id.end()) {
		id_to_material[it->second] = material;
		return it->second;
	}

	if (id_to_material.size() >= UINT16_MAX)
		throw std::runtime_error(
			"Material registry reached uint16_t capacity."
		);

	uint16_t new_id = static_cast<uint16_t>(id_to_material.size());
	key_to_id[key] = new_id;
	id_to_material.push_back(material);
	return new_id;
}

bool MaterialRegistry::hasMaterial(const std::string &key) const {
	return key_to_id.contains(key);
}

uint16_t MaterialRegistry::getMaterialId(const std::string &key) const {
	auto it = key_to_id.find(key);
	if (it == key_to_id.end())
		throw std::runtime_error("Material key not found: " + key);
	return it->second;
}

const Material &MaterialRegistry::getMaterial(const std::string &key) const {
	return getMaterial(getMaterialId(key));
}

const Material &MaterialRegistry::getMaterial(uint16_t material_id) const {
	if (material_id >= id_to_material.size())
		throw std::runtime_error("Material ID out of range.");
	return id_to_material[material_id];
}

const Material &MaterialRegistry::getMaterial(uint8_t material_id) const {
	return getMaterial(static_cast<uint16_t>(material_id));
}

MaterialRegistry::MaterialRegistry() {
	registerMaterial("core:none", Material());
}

size_t MaterialRegistry::materialCount() const {
	return id_to_material.size();
}

const Material *MaterialRegistry::material_data() const {
	return id_to_material.data();
}

}  // namespace lili
