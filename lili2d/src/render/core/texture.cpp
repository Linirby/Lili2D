#include <stdexcept>
#include <cstring>
#include <SDL3_image/SDL_image.h>

#include "texture.hpp"

namespace lili {

Texture::Texture(SDL_GPUDevice *device, const std::string &img_path)
	: device(device) {
	SDL_Surface *temp_surface = IMG_Load(img_path.c_str());
	if (!temp_surface)
		throw std::runtime_error("Failed to load image " + img_path);
	SDL_Surface *surface = SDL_ConvertSurface(
		temp_surface, SDL_PIXELFORMAT_ABGR8888
	);
	SDL_DestroySurface(temp_surface);
	initFromSurface(surface);
}

Texture::Texture(
	SDL_GPUDevice *device, const unsigned char *data, unsigned int len
)
	: device(device) {
	SDL_IOStream *io = SDL_IOFromConstMem(data, len);
	if (!io)
		throw std::runtime_error("Failed to create IOStream from memory");
	SDL_Surface *temp_surface = IMG_Load_IO(io, true);
	if (!temp_surface)
		throw std::runtime_error("Failed to load image from memory");
	SDL_Surface *surface = SDL_ConvertSurface(
		temp_surface, SDL_PIXELFORMAT_ABGR8888
	);
	SDL_DestroySurface(temp_surface);
	initFromSurface(surface);
}

Texture::Texture(SDL_GPUDevice *device, SDL_Surface *surface) {
	this->device = device;
	initFromSurface(surface);
}

Texture::Texture(Texture &&other) noexcept
	: device(other.device),
	width(other.width),
	height(other.height),
	texture(other.texture),
	sampler(other.sampler) {
	other.device = nullptr;
	other.texture = nullptr;
	other.sampler = nullptr;
}

Texture& Texture::operator=(Texture &&other) noexcept {
	if (this != &other) {
		if (sampler) SDL_ReleaseGPUSampler(device, sampler);
		if (texture) SDL_ReleaseGPUTexture(device, texture);

		device = other.device;
		width = other.width;
		height = other.height;
		texture = other.texture;
		sampler = other.sampler;

		other.device = nullptr;
		other.texture = nullptr;
		other.sampler = nullptr;
	}
	return *this;
}

void Texture::initFromSurface(SDL_Surface *surface) {
	width = surface->w;
	height = surface->h;

	SDL_GPUTextureCreateInfo texture_ci{};
	texture_ci.type = SDL_GPU_TEXTURETYPE_2D;
	texture_ci.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
	texture_ci.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
	texture_ci.width = static_cast<uint32_t>(surface->w);
	texture_ci.height = static_cast<uint32_t>(surface->h);
	texture_ci.layer_count_or_depth = 1;
	texture_ci.num_levels = 1;
	texture_ci.sample_count = SDL_GPU_SAMPLECOUNT_1;

	texture = SDL_CreateGPUTexture(this->device, &texture_ci);
	transferToGpu(surface);

	SDL_GPUSamplerCreateInfo sampler_info{};
	sampler_info.min_filter = SDL_GPU_FILTER_NEAREST;
	sampler_info.mag_filter = SDL_GPU_FILTER_NEAREST;
	sampler_info.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_NEAREST,
	sampler_info.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
	sampler_info.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
	sampler_info.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;

	sampler = SDL_CreateGPUSampler(this->device, &sampler_info);
	if (!sampler) {
		throw std::runtime_error(
			"res.texture_sampler creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
}

Texture::~Texture() {
	if (sampler) SDL_ReleaseGPUSampler(device, sampler);
	if (texture) SDL_ReleaseGPUTexture(device, texture);
}

int Texture::getWidth() const {
	return width;
}

int Texture::getHeight() const {
	return height;
}

SDL_GPUTexture *Texture::getTexture() const {
	return texture;
}

SDL_GPUSampler *Texture::getSampler() const {
	return sampler;
}

void Texture::transferToGpu(SDL_Surface *surface) {
	uint32_t image_size = surface->w * surface->h * 4;
	SDL_GPUTransferBufferCreateInfo transfer_bi{};
	transfer_bi.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
	transfer_bi.size = image_size;

	SDL_GPUTransferBuffer *transfer_buffer = SDL_CreateGPUTransferBuffer(
		device, &transfer_bi
	);
	void *map = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
	std::memcpy(map, surface->pixels, image_size);
	SDL_UnmapGPUTransferBuffer(device, transfer_buffer);
	SDL_GPUCommandBuffer *cmd = SDL_AcquireGPUCommandBuffer(device);
	SDL_GPUCopyPass *copy_pass = SDL_BeginGPUCopyPass(cmd);
	SDL_GPUTextureTransferInfo src{};
	src.transfer_buffer = transfer_buffer;
	src.offset = 0;

	SDL_GPUTextureRegion dst{};
	dst.texture = texture;
	dst.w = static_cast<uint32_t>(surface->w);
	dst.h = static_cast<uint32_t>(surface->h);
	dst.d = 1;

	SDL_UploadToGPUTexture(copy_pass, &src, &dst, false);
	SDL_EndGPUCopyPass(copy_pass);
	SDL_SubmitGPUCommandBuffer(cmd);
	SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
	SDL_DestroySurface(surface);
}

}  // namespace lili
