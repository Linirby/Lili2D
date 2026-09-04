#pragma once

#include <map>
#include <vector>

#include <SDL3/SDL_gpu.h>

#include "lili2d/render/passes/pass_types.hpp"

namespace lili {

/// @brief Unified rendering pass for drawing layers.
class MainRenderPass {
public:
    /// @brief Constructs a render pass.
    /// @param pipeline The graphics pipeline to use.
    explicit MainRenderPass(SDL_GPUGraphicsPipeline* pipeline);
    /// @brief Default destructor.
    ~MainRenderPass() = default;

    /// @brief Renders the queued commands.
    /// @param pass The render pass.
    /// @param cmd The command buffer.
    /// @param proj_view The projection-view matrix.
    /// @param queue The list of draw commands.
    void
    render(
        SDL_GPURenderPass* pass, SDL_GPUCommandBuffer* cmd,
        const Mat3& proj_view,
        const std::map<float, std::vector<DrawCommand>>& queue
    );

private:
    SDL_GPUGraphicsPipeline* pipeline = nullptr;
};

}  // namespace lili
