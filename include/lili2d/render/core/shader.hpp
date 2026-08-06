#pragma once

#include <SDL3/SDL_gpu.h>

#include <memory>
#include <string>
#include <vector>

#include "lili2d/core/sdl_deleters.hpp"

namespace lili {

/// @brief Information about loaded shader code.
struct CodeInfo {
    size_t size = 0;           ///< Size of the code buffer.
    std::vector<char> buffer;  ///< The code buffer.
};

/// @brief Information about shader bindings.
struct ShaderInfo {
    uint32_t num_samplers = 0;          ///< Number of samplers.
    uint32_t num_storage_textures = 0;  ///< Number of storage textures.
    uint32_t num_storage_buffers = 0;   ///< Number of storage buffers.
    uint32_t num_uniform_buffers = 0;   ///< Number of uniform buffers.
};

/// @brief Represents a compiled shader program.
class Shader {
public:
    /// @brief Constructs a shader from files.
    /// @param device The SDL GPU device.
    /// @param vert_path Path to the vertex shader file.
    /// @param frag_path Path to the fragment shader file.
    /// @param vert_infos Binding info for the vertex shader.
    /// @param frag_infos Binding info for the fragment shader.
    Shader(
        SDL_GPUDevice* device, const std::string& vert_path,
        const std::string& frag_path,
        ShaderInfo vert_infos = ShaderInfo({.num_uniform_buffers = 1}),
        ShaderInfo frag_infos = ShaderInfo({.num_samplers = 1})
    );
    /// @brief Constructs a shader from memory.
    /// @param device The SDL GPU device.
    /// @param vert_code Pointer to the vertex shader code.
    /// @param vert_size Size of the vertex shader code.
    /// @param frag_code Pointer to the fragment shader code.
    /// @param frag_size Size of the fragment shader code.
    /// @param vert_infos Binding info for the vertex shader.
    /// @param frag_infos Binding info for the fragment shader.
    Shader(
        SDL_GPUDevice* device, const uint8_t* vert_code, size_t vert_size,
        const uint8_t* frag_code, size_t frag_size,
        ShaderInfo vert_infos = ShaderInfo({.num_uniform_buffers = 1}),
        ShaderInfo frag_infos = ShaderInfo({.num_samplers = 1})
    );
    /// @brief Destructor.
    ~Shader() = default;

    /// @brief Move constructor.
    Shader(Shader&& other) noexcept = default;
    /// @brief Move assignment operator.
    /// @param other Shader instance to move from.
    /// @return Reference to this Shader.
    Shader&
    operator=(Shader&& other) noexcept = default;

    /// @brief Copy constructor is deleted to prevent double-freeing the
    /// compiled shader programs.
    Shader(const Shader&) = delete;
    /// @brief Copy assignment is deleted to prevent double-freeing the
    /// compiled shader programs.
    Shader&
    operator=(const Shader&) = delete;

    /// @brief Gets the underlying SDL GPU vertex shader.
    /// @return Pointer to the vertex shader.
    SDL_GPUShader*
    getVertex() const;
    /// @brief Gets the underlying SDL GPU fragment shader.
    /// @return Pointer to the fragment shader.
    SDL_GPUShader*
    getFragment() const;

private:
    /// @brief Pointer to the parent SDL_GPUDevice.
    SDL_GPUDevice* device = nullptr;
    /// @brief Unique pointer to compiled vertex shader handle.
    std::unique_ptr<SDL_GPUShader, SDLGPUShaderDeleter> vertex_shader;
    /// @brief Unique pointer to compiled fragment shader handle.
    std::unique_ptr<SDL_GPUShader, SDLGPUShaderDeleter> fragment_shader;

    /// @brief Reads shader byte code from file into a buffer.
    /// @param code_path Path to the compiled shader file (.spv).
    /// @return CodeInfo structure containing buffer and size.
    CodeInfo
    getCodeInfo(const std::string& code_path);
};

}  // namespace lili
