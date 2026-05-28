#pragma once

#include "render/passes/pass_types.hpp"
#include <map>

namespace lili {

/**
 * \brief Rendering pass for the 2D world.
 */
class World2DPass {
public:
	/**
	 * \brief Constructs a 2D world pass.
	 * \param device The SDL GPU device.
	 * \param pipeline The graphics pipeline to use.
	 */
	World2DPass(SDL_GPUDevice *device, SDL_GPUGraphicsPipeline *pipeline);
	/// \brief Default destructor.
	~World2DPass() = default;

	/**
	 * \brief Renders the queued commands.
	 * \param pass The render pass.
	 * \param cmd The command buffer.
	 * \param proj_view The projection-view matrix.
	 * \param queue The list of draw commands.
	 */
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
