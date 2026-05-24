#pragma once

#include "render/passes/pass_types.hpp"

namespace lili {

class World2DPass {
public:
	World2DPass(SDL_GPUDevice *device, SDL_GPUGraphicsPipeline *pipeline);
	~World2DPass();

	void render(
		SDL_GPURenderPass *pass,
		SDL_GPUCommandBuffer *cmd,
		const Mat3 &proj_view,
		const std::vector<DrawCommand> &queue
	);

private:
	SDL_GPUDevice *device = nullptr;
	SDL_GPUGraphicsPipeline *pipeline = nullptr;
	SDL_GPUBuffer *materials_buffer = nullptr;
};

}  // namespace lili
