#include "lili2d/render/scene/common/material.hpp"

#include "lili2d/render/core/texture.hpp"
#include "lili2d/render/pipelines/main_graphics_pipeline.hpp"

namespace lili {

Material::Material(Texture* texture)
    : albedoMap(texture), custom_pipeline(nullptr), pipeline(nullptr) {}

Material::Material(Texture* texture, SDL_GPUGraphicsPipeline* pipeline)
    : albedoMap(texture), custom_pipeline(pipeline), pipeline(nullptr) {}

Material::Material(Texture* texture, MainGraphicsPipeline* pipeline)
    : albedoMap(texture), custom_pipeline(nullptr), pipeline(pipeline) {}

SDL_GPUGraphicsPipeline*
Material::getPipeline() const {
    if (pipeline) return pipeline->getSdlPipeline();
    return custom_pipeline;
}

}  // namespace lili
