#include <fstream>

#include "shader.hpp"

namespace lili {

Shader::Shader(
	SDL_GPUDevice *device,
	const std::string &vert_path,
	const std::string &frag_path,
	ShaderInfo vert_infos,
	ShaderInfo frag_infos
)
	: device(device) {
	CodeInfo vertex_code_info = getCodeInfo(vert_path);
	SDL_GPUShaderCreateInfo vertex_ci{};
	vertex_ci.code_size = vertex_code_info.size;
	vertex_ci.code = reinterpret_cast<uint8_t *>(vertex_code_info.buffer.data());
	vertex_ci.entrypoint = "main";
	vertex_ci.format = SDL_GPU_SHADERFORMAT_SPIRV;
	vertex_ci.stage = SDL_GPU_SHADERSTAGE_VERTEX;
	vertex_ci.num_samplers = vert_infos.num_samplers;
	vertex_ci.num_storage_textures = vert_infos.num_storage_textures;
	vertex_ci.num_storage_buffers = vert_infos.num_storage_buffers;
	vertex_ci.num_uniform_buffers = vert_infos.num_uniform_buffers;

	vertex_shader = SDL_CreateGPUShader(this->device, &vertex_ci);
	if (!vertex_shader) {
		throw std::runtime_error(
			"vertex_shader creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}

	CodeInfo fragment_code_info = getCodeInfo(frag_path);
	SDL_GPUShaderCreateInfo fragment_ci{};
	fragment_ci.code_size = fragment_code_info.size;
	fragment_ci.code = (
		reinterpret_cast<uint8_t *>(fragment_code_info.buffer.data())
	);
	fragment_ci.entrypoint = "main";
	fragment_ci.format = SDL_GPU_SHADERFORMAT_SPIRV;
	fragment_ci.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
	fragment_ci.num_samplers = frag_infos.num_samplers;
	fragment_ci.num_storage_textures = frag_infos.num_storage_textures;
	fragment_ci.num_storage_buffers = frag_infos.num_storage_buffers;
	fragment_ci.num_uniform_buffers = frag_infos.num_uniform_buffers;

	fragment_shader = SDL_CreateGPUShader(this->device, &fragment_ci);
	if (!fragment_shader) {
		throw std::runtime_error(
			"fragment_shader creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
}

Shader::Shader(
	SDL_GPUDevice *device,
	const uint8_t *vert_code,
	size_t vert_size,
	const uint8_t *frag_code,
	size_t frag_size,
	ShaderInfo vert_infos,
	ShaderInfo frag_infos
)
	: device(device) {
	SDL_GPUShaderCreateInfo vertex_ci{};
	vertex_ci.code_size = vert_size;
	vertex_ci.code = vert_code;
	vertex_ci.entrypoint = "main";
	vertex_ci.format = SDL_GPU_SHADERFORMAT_SPIRV;
	vertex_ci.stage = SDL_GPU_SHADERSTAGE_VERTEX;
	vertex_ci.num_samplers = vert_infos.num_samplers;
	vertex_ci.num_storage_textures = vert_infos.num_storage_textures;
	vertex_ci.num_storage_buffers = vert_infos.num_storage_buffers;
	vertex_ci.num_uniform_buffers = vert_infos.num_uniform_buffers;

	vertex_shader = SDL_CreateGPUShader(this->device, &vertex_ci);
	if (!vertex_shader) {
		throw std::runtime_error(
			"vertex_shader creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}

	SDL_GPUShaderCreateInfo fragment_ci{};
	fragment_ci.code_size = frag_size;
	fragment_ci.code = frag_code;
	fragment_ci.entrypoint = "main";
	fragment_ci.format = SDL_GPU_SHADERFORMAT_SPIRV;
	fragment_ci.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
	fragment_ci.num_samplers = frag_infos.num_samplers;
	fragment_ci.num_storage_textures = frag_infos.num_storage_textures;
	fragment_ci.num_storage_buffers = frag_infos.num_storage_buffers;
	fragment_ci.num_uniform_buffers = frag_infos.num_uniform_buffers;

	fragment_shader = SDL_CreateGPUShader(this->device, &fragment_ci);
	if (!fragment_shader) {
		throw std::runtime_error(
			"fragment_shader creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
}

Shader::~Shader() {
	if (fragment_shader)
		SDL_ReleaseGPUShader(device, fragment_shader);
	if (vertex_shader)
		SDL_ReleaseGPUShader(device, vertex_shader);
}

Shader::Shader(Shader &&other) noexcept
	: device(other.device),
			vertex_shader(other.vertex_shader),
			fragment_shader(other.fragment_shader) {
	other.vertex_shader = nullptr;
	other.fragment_shader = nullptr;
}

Shader& Shader::operator=(Shader &&other) noexcept {
	if (this != &other) {
		if (fragment_shader)
			SDL_ReleaseGPUShader(device, fragment_shader);
		if (vertex_shader)
			SDL_ReleaseGPUShader(device, vertex_shader);
		device = other.device;
		vertex_shader = other.vertex_shader;
		fragment_shader = other.fragment_shader;
		other.vertex_shader = nullptr;
		other.fragment_shader = nullptr;
	}
	return *this;
}

SDL_GPUShader *Shader::getVertex() const {
	return vertex_shader;
}

SDL_GPUShader *Shader::getFragment() const {
	return fragment_shader;
}

CodeInfo Shader::getCodeInfo(const std::string &code_path) {
	CodeInfo code_info;

	std::ifstream file(
		code_path, std::ios::ate | std::ios::binary
	);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file! \"" + code_path + "\"");
	}
	code_info.size = file.tellg();
	file.seekg(0);
	code_info.buffer.resize(code_info.size);
	file.read(code_info.buffer.data(), code_info.size);
	file.close();
	return code_info;
}

}  // namespace lili
