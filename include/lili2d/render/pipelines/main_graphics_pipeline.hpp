#pragma once

#include <SDL3/SDL_gpu.h>
#include <memory>
#include "lili2d/core/sdl_deleters.hpp"

namespace lili {

/// @brief Represents a compiled shader program.
class Shader;

/// @brief Unified graphics pipeline.
class MainGraphicsPipeline {
public:
	/// @brief Constructs the MainGraphicsPipeline.
	MainGraphicsPipeline() = default;

	/// @brief Move constructor.
	MainGraphicsPipeline(MainGraphicsPipeline &&other) noexcept = default;
	/// @brief Move assignment.
	MainGraphicsPipeline& operator=(MainGraphicsPipeline &&other)
		noexcept = default;

	/// @brief Copy constructor is deleted.
	MainGraphicsPipeline(const MainGraphicsPipeline &) = delete;
	/// @brief Copy assignment is deleted.
	MainGraphicsPipeline& operator=(const MainGraphicsPipeline &) = delete;

	/// @brief Constructs the main graphics pipeline.
	/// @param device The SDL GPU device.
	/// @param window The window to render to.
	/// @param shader The shader program.
	MainGraphicsPipeline(
		SDL_GPUDevice *device, SDL_Window *window, Shader *shader
	);
	/// @brief Destructor.
	~MainGraphicsPipeline() = default;

	/// @brief Gets the underlying SDL GPU pipeline.
	/// @return Pointer to the SDL_GPUGraphicsPipeline.
	SDL_GPUGraphicsPipeline *getSdlPipeline();

private:
	SDL_GPUDevice *device = nullptr;
	SDL_Window *window = nullptr;
	Shader *shader = nullptr;

	std::unique_ptr<
		SDL_GPUGraphicsPipeline, SDLGPUGraphicsPipelineDeleter
	> pipeline;
};

}  // namespace lili
