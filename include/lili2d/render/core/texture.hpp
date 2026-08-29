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
    explicit Texture(SDL_GPUDevice* device, const std::string& img_path);
    /// @brief Constructs a texture from memory data.
    /// @param device The SDL GPU device.
    /// @param data Pointer to the image data.
    /// @param len Length of the image data.
    explicit Texture(
        SDL_GPUDevice* device, const unsigned char* data, unsigned int len
    );
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
    explicit Texture(SDL_GPUDevice* device, SDL_Surface* surface);

    /// @brief Gets the width of the texture.
    /// @return The width in pixels.
    [[nodiscard]] inline int
    getWidth() const noexcept {
        return width;
    }
    /// @brief Gets the height of the texture.
    /// @return The height in pixels.
    [[nodiscard]] inline int
    getHeight() const noexcept {
        return height;
    }
    /// @brief Gets the underlying SDL GPU texture.
    /// @return Pointer to the SDL_GPUTexture.
    [[nodiscard]] inline SDL_GPUTexture*
    getTexture() const noexcept {
        return texture.get();
    }
    /// @brief Gets the underlying SDL GPU sampler.
    /// @return Pointer to the SDL_GPUSampler.
    [[nodiscard]] inline SDL_GPUSampler*
    getSampler() const noexcept {
        return sampler.get();
    }

private:
    SDL_GPUDevice* device = nullptr;

    int width = 0;
    int height = 0;
    std::unique_ptr<SDL_GPUTexture, SDLGPUTextureDeleter> texture;
    std::unique_ptr<SDL_GPUSampler, SDLGPUSamplerDeleter> sampler;

    void
    initFromSurface(SDL_Surface* surface);

    void
    transferToGpu(SDL_Surface* surface);
};

}  // namespace lili
