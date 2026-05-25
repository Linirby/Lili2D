#pragma once

#include <SDL3/SDL_gpu.h>
#include <string>

namespace lili {

class Texture {
public:
	Texture(SDL_GPUDevice *device, const std::string &img_path);
	Texture(SDL_GPUDevice *device, const unsigned char *data, unsigned int len);
	~Texture();

	int get_width() const;
	int get_height() const;
	SDL_GPUTexture *get_texture() const;
	SDL_GPUSampler *get_sampler() const;

private:
	SDL_GPUDevice *device = nullptr;

	int width, height;
	SDL_GPUTexture *texture = nullptr;
	SDL_GPUSampler *sampler = nullptr;

	void init_from_surface(SDL_Surface *surface);

	void transfer_to_gpu(SDL_Surface *surface);
};

}  // namespace lili
