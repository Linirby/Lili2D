#pragma once

#include <SDL3/SDL_gpu.h>

namespace lili {

/// @brief Represents Shader.
class Shader;

/**
 * \brief Graphics pipeline for World and 2D world rendering.
 */
class WorldPipeline {
public:
	/**
	 * \brief Constructs the World pipeline.
	 * \param device The SDL GPU device.
	 * \param window The window to render to.
	 * \param shader The shader program.
	 */
	WorldPipeline(SDL_GPUDevice *device, SDL_Window *window, Shader *shader);
	/// \brief Destructor.
	~WorldPipeline();

	/**
	 * \brief Gets the underlying SDL GPU pipeline.
	 * \return Pointer to the SDL_GPUGraphicsPipeline.
	 */
	SDL_GPUGraphicsPipeline *get_sdl_pipeline();

private:
	SDL_GPUDevice *device = nullptr;
	SDL_Window *window = nullptr;
	Shader *shader = nullptr;

	SDL_GPUGraphicsPipeline *pipeline = nullptr;
};

}  // namespace lili
