#include "lili2d/render/scene/common/material.hpp"

namespace lili {

Material::Material(Texture *texture)
	: albedoMap(texture), custom_pipeline(nullptr) {}

Material::Material(Texture *texture, SDL_GPUGraphicsPipeline *pipeline)
	: albedoMap(texture), custom_pipeline(pipeline) {}

}  // namespace lili
