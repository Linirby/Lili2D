#include "lili2d/core/sdl_deleters.hpp"

namespace lili {

void SDLGPUDeviceDeleter::operator()(SDL_GPUDevice* dev) const {
	if (dev)
		SDL_DestroyGPUDevice(dev);
}

SDLGPUDeviceDependentDeleter::SDLGPUDeviceDependentDeleter(SDL_GPUDevice* dev)
	: device(dev) {}

void SDLGPUTextureDeleter::operator()(SDL_GPUTexture* tex) const {
	if (tex && device)
		SDL_ReleaseGPUTexture(device, tex);
}

void SDLGPUSamplerDeleter::operator()(SDL_GPUSampler* sampler) const {
	if (sampler && device)
		SDL_ReleaseGPUSampler(device, sampler);
}

void SDLGPUBufferDeleter::operator()(SDL_GPUBuffer* buffer) const {
	if (buffer && device)
		SDL_ReleaseGPUBuffer(device, buffer);
}

void SDLGPUShaderDeleter::operator()(SDL_GPUShader* shader) const {
	if (shader && device)
		SDL_ReleaseGPUShader(device, shader);
}

void SDLGPUGraphicsPipelineDeleter::operator()(
	SDL_GPUGraphicsPipeline* pipeline
) const {
	if (pipeline && device)
		SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
}

}  // namespace lili
