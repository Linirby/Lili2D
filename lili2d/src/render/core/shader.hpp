#pragma once

#include <SDL3/SDL_gpu.h>
#include <vector>
#include <string>

namespace lili {

/**
 * \brief Information about loaded shader code.
 */
struct CodeInfo {
	size_t size;              ///< Size of the code buffer.
	std::vector<char> buffer; ///< The code buffer.
};

/**
 * \brief Information about shader bindings.
 */
struct ShaderInfo {
	uint32_t num_samplers = 0;         ///< Number of samplers.
	uint32_t num_storage_textures = 0; ///< Number of storage textures.
	uint32_t num_storage_buffers = 0;  ///< Number of storage buffers.
	uint32_t num_uniform_buffers = 0;  ///< Number of uniform buffers.
};

/**
 * \brief Represents a compiled shader program.
 */
class Shader {
public:
	/**
	 * \brief Constructs a shader from files.
	 * \param device The SDL GPU device.
	 * \param vert_path Path to the vertex shader file.
	 * \param frag_path Path to the fragment shader file.
	 * \param vert_infos Binding info for the vertex shader.
	 * \param frag_infos Binding info for the fragment shader.
	 */
	Shader(
		SDL_GPUDevice *device,
		const std::string &vert_path,
		const std::string &frag_path,
		ShaderInfo vert_infos = {},
		ShaderInfo frag_infos = {}
	);
	/**
	 * \brief Constructs a shader from memory.
	 * \param device The SDL GPU device.
	 * \param vert_code Pointer to the vertex shader code.
	 * \param vert_size Size of the vertex shader code.
	 * \param frag_code Pointer to the fragment shader code.
	 * \param frag_size Size of the fragment shader code.
	 * \param vert_infos Binding info for the vertex shader.
	 * \param frag_infos Binding info for the fragment shader.
	 */
	Shader(
		SDL_GPUDevice *device,
		const uint8_t *vert_code,
		size_t vert_size,
		const uint8_t *frag_code,
		size_t frag_size,
		ShaderInfo vert_infos = {},
		ShaderInfo frag_infos = {}
	);
	/// \brief Destructor.
	~Shader();

	/**
	 * \brief Gets the underlying SDL GPU vertex shader.
	 * \return Pointer to the vertex shader.
	 */
	SDL_GPUShader *get_vertex() const;
	/**
	 * \brief Gets the underlying SDL GPU fragment shader.
	 * \return Pointer to the fragment shader.
	 */
	SDL_GPUShader *get_fragment() const;

private:
	SDL_GPUDevice *device = nullptr;
	SDL_GPUShader *vertex_shader = nullptr;
	SDL_GPUShader *fragment_shader = nullptr;

	CodeInfo get_code_info(const std::string &code_path);
};

}  // namespace lili
