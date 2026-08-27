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
    MainGraphicsPipeline(MainGraphicsPipeline&& other) noexcept;
    /// @brief Move assignment operator.
    /// @param other MainGraphicsPipeline instance to move from.
    /// @return Reference to this MainGraphicsPipeline.
    MainGraphicsPipeline&
    operator=(MainGraphicsPipeline&& other) noexcept;

    /// @brief Copy constructor is deleted.
    MainGraphicsPipeline(const MainGraphicsPipeline&) = delete;
    /// @brief Copy assignment is deleted.
    MainGraphicsPipeline&
    operator=(const MainGraphicsPipeline&) = delete;

    /// @brief Constructs the main graphics pipeline.
    /// @param device The SDL GPU device.
    /// @param window The window to render to.
    /// @param shader The shader program.
    MainGraphicsPipeline(
        SDL_GPUDevice* device, SDL_Window* window, Shader* shader
    );
    /// @brief Destructor.
    ~MainGraphicsPipeline();

    /// @brief Rebuilds the underlying graphics pipeline from the current
    /// shader.
    /// @return True if rebuilt successfully, false otherwise.
    bool
    rebuild();

    /// @brief Gets the underlying SDL GPU pipeline.
    /// @return Pointer to the SDL_GPUGraphicsPipeline.
    [[nodiscard]] inline SDL_GPUGraphicsPipeline*
    getSdlPipeline() const noexcept {
        return pipeline.get();
    }

private:
    /// @brief Helper to build the SDL GPU pipeline object.
    bool
    createPipelineInternal();

    /// @brief Pointer to the parent SDL_GPUDevice.
    SDL_GPUDevice* device = nullptr;
    /// @brief Pointer to target SDL_Window.
    SDL_Window* window = nullptr;
    /// @brief Pointer to Shader program.
    Shader* shader = nullptr;

    /// @brief Unique pointer to compiled SDL GPU pipeline handle.
    std::unique_ptr<SDL_GPUGraphicsPipeline, SDLGPUGraphicsPipelineDeleter>
        pipeline;
};

}  // namespace lili
