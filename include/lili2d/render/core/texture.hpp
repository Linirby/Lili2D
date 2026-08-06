#pragma once

#include <SDL3/SDL_gpu.h>

#include <memory>
#include <string>

#include "lili2d/core/sdl_deleters.hpp"

namespace lili {

/// @brief Represents a 2D texture on the GPU.
class Texture {
public:
    /// @brief Constructs a texture from an image file.
    /// @param device The SDL GPU device.
    /// @param img_path Path to the image file.
    Texture(SDL_GPUDevice* device, const std::string& img_path);
    /// @brief Constructs a texture from memory data.
    /// @param device The SDL GPU device.
    /// @param data Pointer to the image data.
    /// @param len Length of the image data.
    Texture(SDL_GPUDevice* device, const unsigned char* data, unsigned int len);
    /// @brief Destructor.
    ~Texture() = default;

    /// @brief Move constructor.
    Texture(Texture&& other) noexcept = default;
    /// @brief Move assignment operator.
    /// @param other Texture instance to move from.
    /// @return Reference to this Texture.
    Texture&
    operator=(Texture&& other) noexcept = default;

    /// @brief Copy constructor is deleted to prevent double-freeing the GPU
    /// texture.
    Texture(const Texture&) = delete;
    /// @brief Copy assignment is deleted to prevent double-freeing the GPU
    /// texture.
    Texture&
    operator=(const Texture&) = delete;

    /// @brief Constructs a texture directly from an SDL_Surface.
    /// @param device The SDL GPU device.
    /// @param surface The surface (it is NOT destroyed by this constructor).
    Texture(SDL_GPUDevice* device, SDL_Surface* surface);

    /// @brief Gets the width of the texture.
    /// @return The width in pixels.
    int
    getWidth() const;
    /// @brief Gets the height of the texture.
    /// @return The height in pixels.
    int
    getHeight() const;
    /// @brief Gets the underlying SDL GPU texture.
    /// @return Pointer to the SDL_GPUTexture.
    SDL_GPUTexture*
    getTexture() const;
    /// @brief Gets the underlying SDL GPU sampler.
    /// @return Pointer to the SDL_GPUSampler.
    SDL_GPUSampler*
    getSampler() const;

private:
    /// @brief Pointer to the parent SDL_GPUDevice.
    SDL_GPUDevice* device = nullptr;

    /// @brief Texture width in pixels.
    int width = 0;
    /// @brief Texture height in pixels.
    int height = 0;
    /// @brief Unique pointer to GPU texture handle.
    std::unique_ptr<SDL_GPUTexture, SDLGPUTextureDeleter> texture;
    /// @brief Unique pointer to GPU sampler handle.
    std::unique_ptr<SDL_GPUSampler, SDLGPUSamplerDeleter> sampler;

    /// @brief Initializes GPU texture metadata and objects from an SDL_Surface.
    /// @param surface Pointer to source SDL_Surface.
    void
    initFromSurface(SDL_Surface* surface);

    /// @brief Transfers CPU surface pixel data to the GPU texture memory.
    /// @param surface Pointer to source SDL_Surface.
    void
    transferToGpu(SDL_Surface* surface);
};

}  // namespace lili
