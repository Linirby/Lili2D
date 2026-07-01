#pragma once

#include <SDL3/SDL_gpu.h>

namespace lili {

/// @brief Deleter for SDL_GPUDevice
struct SDLGPUDeviceDeleter {
	void operator()(SDL_GPUDevice* dev) const;
};

/// @brief Base class for deleters that require an SDL_GPUDevice pointer
struct SDLGPUDeviceDependentDeleter {
	SDL_GPUDevice* device = nullptr;
	SDLGPUDeviceDependentDeleter() = default;
	SDLGPUDeviceDependentDeleter(SDL_GPUDevice* dev);
};

/// @brief Deleter for SDL_GPUTexture
struct SDLGPUTextureDeleter : SDLGPUDeviceDependentDeleter {
	using SDLGPUDeviceDependentDeleter::SDLGPUDeviceDependentDeleter;
	void operator()(SDL_GPUTexture* tex) const;
};

/// @brief Deleter for SDL_GPUSampler
struct SDLGPUSamplerDeleter : SDLGPUDeviceDependentDeleter {
	using SDLGPUDeviceDependentDeleter::SDLGPUDeviceDependentDeleter;
	void operator()(SDL_GPUSampler* sampler) const;
};

/// @brief Deleter for SDL_GPUBuffer
struct SDLGPUBufferDeleter : SDLGPUDeviceDependentDeleter {
	using SDLGPUDeviceDependentDeleter::SDLGPUDeviceDependentDeleter;
	void operator()(SDL_GPUBuffer* buffer) const;
};

/// @brief Deleter for SDL_GPUShader
struct SDLGPUShaderDeleter : SDLGPUDeviceDependentDeleter {
	using SDLGPUDeviceDependentDeleter::SDLGPUDeviceDependentDeleter;
	void operator()(SDL_GPUShader* shader) const;
};

/// @brief Deleter for SDL_GPUGraphicsPipeline
struct SDLGPUGraphicsPipelineDeleter : SDLGPUDeviceDependentDeleter {
	using SDLGPUDeviceDependentDeleter::SDLGPUDeviceDependentDeleter;
	void operator()(SDL_GPUGraphicsPipeline* pipeline) const;
};

}  // namespace lili
