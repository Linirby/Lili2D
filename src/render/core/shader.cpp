#include "lili2d/render/core/shader.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

namespace lili {

static void
ensureShaderCrossInit() {
    static bool initialized = []() {
        if (!SDL_ShaderCross_Init()) {
            throw std::runtime_error(
                "SDL_ShaderCross_Init failed: " + std::string(SDL_GetError())
            );
        }
        return true;
    }();
    (void)initialized;
}

std::string
Shader::readFile(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        throw std::runtime_error(
            "Failed to open shader file: \"" + file_path + "\""
        );
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

SDL_GPUShader*
Shader::compileHLSL(
    SDL_GPUDevice* device, const std::string& source,
    const std::string& entrypoint, SDL_ShaderCross_ShaderStage stage
) {
    if (!device) {
        throw std::runtime_error(
            "Cannot compile shader: SDL_GPUDevice is null!"
        );
    }

    ensureShaderCrossInit();

    SDL_ShaderCross_HLSL_Info hlsl_info{};
    hlsl_info.source = source.c_str();
    hlsl_info.entrypoint = entrypoint.c_str();
    hlsl_info.shader_stage = stage;

    size_t spirv_size = 0;
    void* spirv_data =
        SDL_ShaderCross_CompileSPIRVFromHLSL(&hlsl_info, &spirv_size);
    if (!spirv_data) {
        throw std::runtime_error(
            "HLSL to SPIR-V compilation failed for entrypoint '" + entrypoint +
            "':\n-> " + std::string(SDL_GetError())
        );
    }

    SDL_ShaderCross_GraphicsShaderMetadata* metadata =
        SDL_ShaderCross_ReflectGraphicsSPIRV(
            static_cast<const Uint8*>(spirv_data), spirv_size, 0
        );

    SDL_ShaderCross_SPIRV_Info spirv_info{};
    spirv_info.bytecode = static_cast<const Uint8*>(spirv_data);
    spirv_info.bytecode_size = spirv_size;
    spirv_info.entrypoint = entrypoint.c_str();
    spirv_info.shader_stage = stage;

    SDL_GPUShader* gpu_shader = SDL_ShaderCross_CompileGraphicsShaderFromSPIRV(
        device, &spirv_info, metadata ? &metadata->resource_info : nullptr, 0
    );

    if (metadata) {
        SDL_free(metadata);
    }
    SDL_free(spirv_data);

    if (!gpu_shader) {
        throw std::runtime_error(
            "SDL_ShaderCross_CompileGraphicsShaderFromSPIRV failed for "
            "entrypoint '" +
            entrypoint + "':\n-> " + std::string(SDL_GetError())
        );
    }

    return gpu_shader;
}

Shader::Shader(
    SDL_GPUDevice* device, const std::string& vert_path,
    const std::string& frag_path, const std::string& vert_entry,
    const std::string& frag_entry
)
    : device(device) {
    std::string vert_source = readFile(vert_path);
    std::string frag_source = readFile(frag_path);

    vertex_shader = std::unique_ptr<SDL_GPUShader, SDLGPUShaderDeleter>(
        compileHLSL(
            device, vert_source, vert_entry, SDL_SHADERCROSS_SHADERSTAGE_VERTEX
        ),
        SDLGPUShaderDeleter(device)
    );

    fragment_shader = std::unique_ptr<SDL_GPUShader, SDLGPUShaderDeleter>(
        compileHLSL(
            device, frag_source, frag_entry,
            SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT
        ),
        SDLGPUShaderDeleter(device)
    );
}

std::unique_ptr<Shader>
Shader::fromSource(
    SDL_GPUDevice* device, std::string_view vert_source,
    std::string_view frag_source, const std::string& vert_entry,
    const std::string& frag_entry
) {
    SDL_GPUShader* vs = compileHLSL(
        device, std::string(vert_source), vert_entry,
        SDL_SHADERCROSS_SHADERSTAGE_VERTEX
    );
    SDL_GPUShader* fs = nullptr;
    try {
        fs = compileHLSL(
            device, std::string(frag_source), frag_entry,
            SDL_SHADERCROSS_SHADERSTAGE_FRAGMENT
        );
    } catch (...) {
        if (vs) SDL_ReleaseGPUShader(device, vs);
        throw;
    }

    return std::make_unique<Shader>(device, vs, fs);
}

std::unique_ptr<Shader>
Shader::fromFiles(
    SDL_GPUDevice* device, const std::string& vert_path,
    const std::string& frag_path, const std::string& vert_entry,
    const std::string& frag_entry
) {
    return std::make_unique<Shader>(
        device, vert_path, frag_path, vert_entry, frag_entry
    );
}

Shader::Shader(SDL_GPUDevice* device, SDL_GPUShader* vert, SDL_GPUShader* frag)
    : device(device),
      vertex_shader(vert, SDLGPUShaderDeleter(device)),
      fragment_shader(frag, SDLGPUShaderDeleter(device)) {}

Shader::~Shader() { reload_listeners.clear(); }

Shader::Shader(Shader&& other) noexcept
    : device(other.device),
      vertex_shader(std::move(other.vertex_shader)),
      fragment_shader(std::move(other.fragment_shader)),
      reload_listeners(std::move(other.reload_listeners)) {
    other.device = nullptr;
}

Shader&
Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        device = other.device;
        vertex_shader = std::move(other.vertex_shader);
        fragment_shader = std::move(other.fragment_shader);
        other.device = nullptr;
        notifyReloaded();
    }
    return *this;
}

void
Shader::addReloadListener(void* owner, ReloadCallback callback) {
    if (owner && callback) reload_listeners[owner] = std::move(callback);
}

void
Shader::removeReloadListener(void* owner) {
    reload_listeners.erase(owner);
}

void
Shader::notifyReloaded() {
    std::unordered_map<void*, ReloadCallback> listeners = reload_listeners;
    for (const auto& [owner, callback] : listeners)
        if (callback) callback();
}

SDL_GPUShader*
Shader::getVertex() const {
    return vertex_shader.get();
}

SDL_GPUShader*
Shader::getFragment() const {
    return fragment_shader.get();
}

}  // namespace lili
