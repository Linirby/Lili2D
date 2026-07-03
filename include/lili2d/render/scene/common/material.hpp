#pragma once

#include <cstdint>
#include <vector>

#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/core/texture.hpp"

namespace lili {

/// @brief Properties for a material.
struct MaterialProps {
	Vec4 color_tint = { 1.0f, 1.0f, 1.0f, 1.0f };  ///< Color tint.
	Vec4 uv_bounds = { 0.0f, 0.0f, 1.0f, 1.0f };   ///< UV bounds bounds.
};

/// @brief Defines how a model is rendered.
struct Material {
	MaterialProps properties;  ///< Material properties.
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
	void setVertexUniforms(const T& data);
	/// @brief Sets custom fragment uniforms.
	/// @param data The uniform data struct.
	template<typename T>
	void setFragmentUniforms(const T& data);
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

template<typename T>
void Material::setVertexUniforms(const T& data) {
	const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&data);
	custom_vertex_uniforms.assign(ptr, ptr + sizeof(T));
}

template<typename T>
void Material::setFragmentUniforms(const T& data) {
	const uint8_t* ptr = reinterpret_cast<const uint8_t*>(&data);
	custom_fragment_uniforms.assign(ptr, ptr + sizeof(T));
}

}  // namespace lili
