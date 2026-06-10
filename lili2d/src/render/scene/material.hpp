#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "geometry/vec4.hpp"
#include "render/core/texture.hpp"

namespace lili {

/// @brief Properties for a material.
struct MaterialProps {
	Vec4 color_tint = { 1.0f, 1.0f, 1.0f, 1.0f };  ///< Color tint.
};

/// @brief Defines how a model is rendered.
struct Material {
	MaterialProps properties;       ///< Material properties.
	Texture *albedoMap = nullptr;  ///< Albedo texture map.
	SDL_GPUGraphicsPipeline *custom_pipeline = nullptr;
	///< Custom graphics pipeline.

	std::vector<uint8_t> custom_vertex_uniforms;
	///< Custom vertex uniforms for binding 1.
	std::vector<uint8_t> custom_fragment_uniforms;
	///< Custom fragment uniforms for binding 0.

	/// @brief Sets custom vertex uniforms.
	/// @param data The uniform data struct.
	template<typename T>
	void setVertexUniforms(const T& data) {
		const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&data);
		custom_vertex_uniforms.assign(ptr, ptr + sizeof(T));
	}

	/// @brief Sets custom fragment uniforms.
	/// @param data The uniform data struct.
	template<typename T>
	void setFragmentUniforms(const T& data) {
		const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&data);
		custom_fragment_uniforms.assign(ptr, ptr + sizeof(T));
	}

	/// @brief Default constructor.
	Material() = default;
	/// @brief Constructs a material with an albedo map.
	/// @param texture Pointer to the texture.
	Material(Texture *texture);
	/// @brief Constructs a material with an albedo map and custom pipeline.
	/// @param texture Pointer to the texture.
	/// @param pipeline Pointer to the custom pipeline.
	Material(Texture *texture, SDL_GPUGraphicsPipeline *pipeline);
};

/// @brief Registry for managing materials globally.
class MaterialRegistry {
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
	uint16_t getMaterialId(const std::string &key) const;
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
	const Material *material_data() const;

private:
	std::unordered_map<std::string, uint16_t> key_to_id;
	std::vector<Material> id_to_material;

	/// @brief MaterialRegistry method.
	MaterialRegistry();
};

}  // namespace lili
