#include "lili2d/render/scene/common/material_registry.hpp"

namespace lili {

MaterialRegistry &MaterialRegistry::get() {
	static MaterialRegistry instance;
	return instance;
}

uint16_t MaterialRegistry::registerMaterial(
	const std::string &key, const Material &material
) {
	return registerAsset(key, material);
}

bool MaterialRegistry::hasMaterial(const std::string &key) const {
	return hasAsset(key);
}

uint16_t MaterialRegistry::getMaterialID(const std::string &key) const {
	return getAssetID(key);
}

const Material &MaterialRegistry::getMaterial(const std::string &key) const {
	return getAsset(key);
}

const Material &MaterialRegistry::getMaterial(uint16_t material_id) const {
	return getAsset(material_id);
}

const Material &MaterialRegistry::getMaterial(uint8_t material_id) const {
	return getAsset(material_id);
}

size_t MaterialRegistry::materialCount() const {
	return assetCount();
}

const Material *MaterialRegistry::materialData() const {
	return assetData();
}

MaterialRegistry::MaterialRegistry() {
	registerMaterial("core:none", Material());
}

}
