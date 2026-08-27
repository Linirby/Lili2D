#pragma once

#include <SDL3/SDL_gpu.h>

namespace lili {

/// @brief Deleter for SDL_Surface
struct SDLSurfaceDeleter {
    /// @brief Releases the SDL_Surface pointer.
    /// @param surf Pointer to the SDL_Surface to destroy.
    void
    operator()(SDL_Surface* surf) const;
};

/// @brief Deleter for SDL_GPUDevice
struct SDLGPUDeviceDeleter {
    /// @brief Releases the SDL_GPUDevice resource.
    /// @param dev Pointer to the SDL_GPUDevice to destroy.
    void
    operator()(SDL_GPUDevice* dev) const;
};

/// @brief Base class for deleters that require an SDL_GPUDevice pointer
struct SDLGPUDeviceDependentDeleter {
    /// @brief Pointer to the parent SDL_GPUDevice.
    SDL_GPUDevice* device = nullptr;
    /// @brief Default constructor.
    SDLGPUDeviceDependentDeleter() = default;
    /// @brief Constructs a deleter bound to an SDL_GPUDevice.
    /// @param dev Pointer to the target SDL_GPUDevice.
    explicit SDLGPUDeviceDependentDeleter(SDL_GPUDevice* dev);
};

/// @brief Deleter for SDL_GPUTexture
struct SDLGPUTextureDeleter : SDLGPUDeviceDependentDeleter {
    using SDLGPUDeviceDependentDeleter::SDLGPUDeviceDependentDeleter;
    /// @brief Releases the SDL_GPUTexture resource.
    /// @param tex Pointer to the SDL_GPUTexture to release.
    void
    operator()(SDL_GPUTexture* tex) const;
};

/// @brief Deleter for SDL_GPUSampler
struct SDLGPUSamplerDeleter : SDLGPUDeviceDependentDeleter {
    using SDLGPUDeviceDependentDeleter::SDLGPUDeviceDependentDeleter;
    /// @brief Releases the SDL_GPUSampler resource.
    /// @param sampler Pointer to the SDL_GPUSampler to release.
    void
    operator()(SDL_GPUSampler* sampler) const;
};

/// @brief Deleter for SDL_GPUBuffer
struct SDLGPUBufferDeleter : SDLGPUDeviceDependentDeleter {
    using SDLGPUDeviceDependentDeleter::SDLGPUDeviceDependentDeleter;
    /// @brief Releases the SDL_GPUBuffer resource.
    /// @param buffer Pointer to the SDL_GPUBuffer to release.
    void
    operator()(SDL_GPUBuffer* buffer) const;
};

/// @brief Deleter for SDL_GPUShader
struct SDLGPUShaderDeleter : SDLGPUDeviceDependentDeleter {
    using SDLGPUDeviceDependentDeleter::SDLGPUDeviceDependentDeleter;
    /// @brief Releases the SDL_GPUShader resource.
    /// @param shader Pointer to the SDL_GPUShader to release.
    void
    operator()(SDL_GPUShader* shader) const;
};

/// @brief Deleter for SDL_GPUGraphicsPipeline
struct SDLGPUGraphicsPipelineDeleter : SDLGPUDeviceDependentDeleter {
    using SDLGPUDeviceDependentDeleter::SDLGPUDeviceDependentDeleter;
    /// @brief Releases the SDL_GPUGraphicsPipeline resource.
    /// @param pipeline Pointer to the SDL_GPUGraphicsPipeline to release.
    void
    operator()(SDL_GPUGraphicsPipeline* pipeline) const;
};

}  // namespace lili
