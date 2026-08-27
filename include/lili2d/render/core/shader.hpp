#pragma once

#include <SDL3/SDL_gpu.h>
#include <SDL3_shadercross/SDL_shadercross.h>

#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "lili2d/core/sdl_deleters.hpp"

namespace lili {

/// @brief Information about shader bindings (optional for HLSL, auto-reflected by SDL_ShaderCross).
struct ShaderInfo {
    uint32_t num_samplers = 0;          ///< Number of samplers.
    uint32_t num_storage_textures = 0;  ///< Number of storage textures.
    uint32_t num_storage_buffers = 0;   ///< Number of storage buffers.
    uint32_t num_uniform_buffers = 0;   ///< Number of uniform buffers.
};

/// @brief Represents a compiled shader program using HLSL sources.
class Shader {
public:
    /// @brief Reload listener callback type alias.
    using ReloadCallback = std::function<void()>;

    /// @brief Constructs a shader from HLSL files.
    /// @param device The SDL GPU device.
    /// @param vert_path Path to the vertex shader file (.hlsl).
    /// @param frag_path Path to the fragment shader file (.hlsl).
    /// @param vert_entry Entry point for the vertex shader (default: "main").
    /// @param frag_entry Entry point for the fragment shader (default: "main").
    explicit Shader(
        SDL_GPUDevice* device, const std::string& vert_path,
        const std::string& frag_path,
        const std::string& vert_entry = "main",
        const std::string& frag_entry = "main"
    );

    /// @brief Creates a shader from HLSL source strings.
    /// @param device The SDL GPU device.
    /// @param vert_source Vertex HLSL source code.
    /// @param frag_source Fragment HLSL source code.
    /// @param vert_entry Entry point for the vertex shader (default: "main").
    /// @param frag_entry Entry point for the fragment shader (default: "main").
    /// @return Unique pointer to the created Shader.
    static std::unique_ptr<Shader>
    fromSource(
        SDL_GPUDevice* device,
        std::string_view vert_source,
        std::string_view frag_source,
        const std::string& vert_entry = "main",
        const std::string& frag_entry = "main"
    );

    /// @brief Creates a shader from HLSL files.
    /// @param device The SDL GPU device.
    /// @param vert_path Path to the vertex shader file (.hlsl).
    /// @param frag_path Path to the fragment shader file (.hlsl).
    /// @param vert_entry Entry point for the vertex shader (default: "main").
    /// @param frag_entry Entry point for the fragment shader (default: "main").
    /// @return Unique pointer to the created Shader.
    static std::unique_ptr<Shader>
    fromFiles(
        SDL_GPUDevice* device,
        const std::string& vert_path,
        const std::string& frag_path,
        const std::string& vert_entry = "main",
        const std::string& frag_entry = "main"
    );

    /// @brief Constructs a shader directly from compiled SDL_GPUShader handles.
    /// @param device The SDL GPU device.
    /// @param vert Pointer to vertex SDL_GPUShader.
    /// @param frag Pointer to fragment SDL_GPUShader.
    explicit Shader(
        SDL_GPUDevice* device,
        SDL_GPUShader* vert,
        SDL_GPUShader* frag
    );

    /// @brief Destructor.
    ~Shader();

    /// @brief Move constructor.
    Shader(Shader&& other) noexcept;
    /// @brief Move assignment operator.
    Shader&
    operator=(Shader&& other) noexcept;

    /// @brief Copy constructor is deleted to prevent double-freeing the
    /// compiled shader programs.
    Shader(const Shader&) = delete;
    /// @brief Copy assignment is deleted to prevent double-freeing the
    /// compiled shader programs.
    Shader&
    operator=(const Shader&) = delete;

    /// @brief Registers a reload observer listener.
    /// @param owner Unique pointer identifying the observer.
    /// @param callback Function called when the shader has reloaded.
    void
    addReloadListener(void* owner, ReloadCallback callback);

    /// @brief Unregisters a reload observer listener.
    /// @param owner Unique pointer identifying the observer.
    void
    removeReloadListener(void* owner);

    /// @brief Notifies all registered listeners that the shader has reloaded.
    void
    notifyReloaded();

    /// @brief Gets the underlying SDL GPU vertex shader.
    /// @return Pointer to the vertex shader.
    SDL_GPUShader*
    getVertex() const;
    /// @brief Gets the underlying SDL GPU fragment shader.
    /// @return Pointer to the fragment shader.
    SDL_GPUShader*
    getFragment() const;

    /// @brief Compiles a single HLSL shader stage using SDL_ShaderCross.
    /// @param device The SDL GPU device.
    /// @param source HLSL source code.
    /// @param entrypoint Entry point function name.
    /// @param stage Vertex or Fragment stage.
    /// @return Compiled SDL_GPUShader pointer.
    static SDL_GPUShader*
    compileHLSL(
        SDL_GPUDevice* device,
        const std::string& source,
        const std::string& entrypoint,
        SDL_ShaderCross_ShaderStage stage
    );

private:
    /// @brief Pointer to the parent SDL_GPUDevice.
    SDL_GPUDevice* device = nullptr;
    /// @brief Unique pointer to compiled vertex shader handle.
    std::unique_ptr<SDL_GPUShader, SDLGPUShaderDeleter> vertex_shader;
    /// @brief Unique pointer to compiled fragment shader handle.
    std::unique_ptr<SDL_GPUShader, SDLGPUShaderDeleter> fragment_shader;
    /// @brief Registered reload listeners keyed by owner pointer.
    std::unordered_map<void*, ReloadCallback> reload_listeners;

    /// @brief Reads text file contents into a string.
    /// @param file_path Path to the file.
    /// @return String containing file contents.
    static std::string
    readFile(const std::string& file_path);
};

}  // namespace lili
