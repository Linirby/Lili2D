#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "geometry/vec4.hpp"
#include "render/core/texture.hpp"

namespace lili {

/**
 * \brief Properties for a material.
 */
struct MaterialProps {
	Vec4 color_tint = { 1.0f, 1.0f, 1.0f, 1.0f }; ///< Color tint.
};

/**
 * \brief Defines how a model is rendered.
 */
struct Material {
	MaterialProps properties;      ///< Material properties.
	Texture *albedo_map = nullptr; ///< Albedo texture map.

	/// \brief Default constructor.
	Material() = default;
	/**
	 * \brief Constructs a material with an albedo map.
	 * \param texture Pointer to the texture.
	 */
	Material(Texture *texture);
};

/**
 * \brief Registry for managing materials globally.
 */
class MaterialRegistry {
public:
	MaterialRegistry(const MaterialRegistry&) = delete;
	MaterialRegistry& operator=(const MaterialRegistry&) = delete;

	/**
	 * \brief Gets the singleton instance.
	 * \return Reference to the MaterialRegistry.
	 */
	static MaterialRegistry &get();

	/**
	 * \brief Registers a new material.
	 * \param key The unique string key.
	 * \param material The material to register.
	 * \return The assigned material ID.
	 */
	uint16_t register_material(const std::string &key, const Material &material);

	/**
	 * \brief Checks if a material exists.
	 * \param key The material key.
	 * \return True if the material exists.
	 */
	bool has_material(const std::string &key) const;
	/**
	 * \brief Gets a material ID by key.
	 * \param key The material key.
	 * \return The material ID.
	 */
	uint16_t get_material_id(const std::string &key) const;
	/**
	 * \brief Gets a material by key.
	 * \param key The material key.
	 * \return Reference to the material.
	 */
	const Material &get_material(const std::string &key) const;
	/**
	 * \brief Gets a material by 16-bit ID.
	 * \param material_id The material ID.
	 * \return Reference to the material.
	 */
	const Material &get_material(uint16_t material_id) const;
	/**
	 * \brief Gets a material by 8-bit ID.
	 * \param material_id The material ID.
	 * \return Reference to the material.
	 */
	const Material &get_material(uint8_t material_id) const;

	/**
	 * \brief Gets the total number of registered materials.
	 * \return The number of materials.
	 */
	size_t material_count() const;
	/**
	 * \brief Gets the raw material data array.
	 * \return Pointer to the material data.
	 */
	const Material *material_data() const;

private:
	std::unordered_map<std::string, uint16_t> key_to_id;
	std::vector<Material> id_to_material;

	MaterialRegistry();
};

}  // namespace lili
