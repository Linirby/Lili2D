#pragma once

#include "lili2d/render/passes/pass_types.hpp"
#include <map>

namespace lili {

/// @brief Unified rendering pass for drawing layers.
class MainRenderPass {
public:
	/// @brief Constructs a render pass.
	/// @param device The SDL GPU device.
	/// @param pipeline The graphics pipeline to use.
	MainRenderPass(SDL_GPUDevice *device, SDL_GPUGraphicsPipeline *pipeline);
	/// @brief Default destructor.


	/// @brief Renders the queued commands.
	/// @param pass The render pass.
	/// @param cmd The command buffer.
	/// @param proj_view The projection-view matrix.
	/// @param queue The list of draw commands.
	void render(
		SDL_GPURenderPass *pass,
		SDL_GPUCommandBuffer *cmd,
		const Mat3 &proj_view,
		const std::map<float, std::vector<DrawCommand>> &queue
	);

private:
	SDL_GPUDevice *device = nullptr;
	SDL_GPUGraphicsPipeline *pipeline = nullptr;
};

}  // namespace lili
