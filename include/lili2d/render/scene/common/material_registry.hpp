#pragma once

#include <cstdint>
#include <string>

#include "lili2d/core/asset_registry.hpp"
#include "lili2d/render/scene/common/material.hpp"

namespace lili {

/// @brief Registry for managing materials globally.
class MaterialRegistry : public AssetRegistry<Material, uint16_t> {
public:
	MaterialRegistry(const MaterialRegistry&) = delete;
	MaterialRegistry& operator=(const MaterialRegistry&) = delete;

	/// @brief Gets the singleton instance.
	/// @return Reference to the MaterialRegistry.
	static MaterialRegistry &get();

	/// @brief Registers a new material.
	/// @param key The unique string key.
	/// @param material The material to register.
	/// @return The assigned material ID.
	uint16_t registerMaterial(
		const std::string &key, const Material &material
	);

	/// @brief Checks if a material exists.
	/// @param key The material key.
	/// @return True if the material exists.
	bool hasMaterial(const std::string &key) const;
	/// @brief Gets a material ID by key.
	/// @param key The material key.
	/// @return The material ID.
	uint16_t getMaterialID(const std::string &key) const;
	/// @brief Gets a material by key.
	/// @param key The material key.
	/// @return Reference to the material.
	const Material &getMaterial(const std::string &key) const;
	/// @brief Gets a material by 16-bit ID.
	/// @param material_id The material ID.
	/// @return Reference to the material.
	const Material &getMaterial(uint16_t material_id) const;
	/// @brief Gets a material by 8-bit ID.
	/// @param material_id The material ID.
	/// @return Reference to the material.
	const Material &getMaterial(uint8_t material_id) const;

	/// @brief Gets the total number of registered materials.
	/// @return The number of materials.
	size_t materialCount() const;
	/// @brief Gets the raw material data array.
	/// @return Pointer to the material data.
	const Material *materialData() const;

private:
	/// @brief MaterialRegistry method.
	MaterialRegistry();
};

}
