#pragma once

#include <SDL3/SDL_gpu.h>
#include <string>

namespace lili {

/**
 * \brief Represents a 2D texture on the GPU.
 */
class Texture {
public:
	/**
	 * \brief Constructs a texture from an image file.
	 * \param device The SDL GPU device.
	 * \param img_path Path to the image file.
	 */
	Texture(SDL_GPUDevice *device, const std::string &img_path);
	/**
	 * \brief Constructs a texture from memory data.
	 * \param device The SDL GPU device.
	 * \param data Pointer to the image data.
	 * \param len Length of the image data.
	 */
	Texture(SDL_GPUDevice *device, const unsigned char *data, unsigned int len);
	/// \brief Destructor.
	~Texture();

	/// \brief Move constructor.
	Texture(Texture &&other) noexcept;
	/// \brief Move assignment.
	Texture& operator=(Texture &&other) noexcept;

	/// \brief Copy constructor is deleted to prevent double-freeing the GPU texture.
	Texture(const Texture &) = delete;
	/// \brief Copy assignment is deleted to prevent double-freeing the GPU texture.
	Texture& operator=(const Texture &) = delete;

	/**
	 * \brief Constructs a texture directly from an SDL_Surface.
	 * \param device The SDL GPU device.
	 * \param surface The surface (will be destroyed after upload).
	 */
	Texture(SDL_GPUDevice *device, SDL_Surface *surface);


	/**
	 * \brief Gets the width of the texture.
	 * \return The width in pixels.
	 */
	int get_width() const;
	/**
	 * \brief Gets the height of the texture.
	 * \return The height in pixels.
	 */
	int get_height() const;
	/**
	 * \brief Gets the underlying SDL GPU texture.
	 * \return Pointer to the SDL_GPUTexture.
	 */
	SDL_GPUTexture *get_texture() const;
	/**
	 * \brief Gets the underlying SDL GPU sampler.
	 * \return Pointer to the SDL_GPUSampler.
	 */
	SDL_GPUSampler *get_sampler() const;

private:
	SDL_GPUDevice *device = nullptr;

	int width, height;
	SDL_GPUTexture *texture = nullptr;
	SDL_GPUSampler *sampler = nullptr;

	/// \brief init_from_surface method.
	void init_from_surface(SDL_Surface *surface);

	/// \brief transfer_to_gpu method.
	void transfer_to_gpu(SDL_Surface *surface);
};

}  // namespace lili
