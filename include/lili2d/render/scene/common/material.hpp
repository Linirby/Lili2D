#pragma once

#include <cstdint>
#include <vector>

#include "lili2d/geometry/vec4.hpp"
#include "lili2d/render/core/texture.hpp"
#include "lili2d/render/pipelines/main_graphics_pipeline.hpp"

struct SDL_GPUGraphicsPipeline;

namespace lili {

/// @brief Properties for a material.
struct MaterialProps
{
    Vec4 color_tint = { 1.0f, 1.0f, 1.0f, 1.0f }; ///< Color tint.
    Vec4 uv_bounds = { 0.0f, 0.0f, 1.0f, 1.0f };  ///< UV bounds bounds.
};

/// @brief Defines how a model is rendered.
struct Material
{
    MaterialProps properties;     ///< Material properties.
    Texture* albedoMap = nullptr; ///< Albedo texture map.
    SDL_GPUGraphicsPipeline* custom_pipeline = nullptr;
    ///< Custom raw graphics pipeline.
    MainGraphicsPipeline* pipeline = nullptr;
    ///< Custom graphics pipeline wrapper (supports hot-reloading).

    std::vector<uint8_t> custom_vertex_uniforms;
    ///< Custom vertex uniforms for binding 1.
    std::vector<uint8_t> custom_fragment_uniforms;
    ///< Custom fragment uniforms for binding 0.

    /// @brief Gets the active SDL GPU pipeline handle.
    /// @return Pointer to SDL_GPUGraphicsPipeline, or nullptr.
    [[nodiscard]] inline SDL_GPUGraphicsPipeline*
    getPipeline() const noexcept
    {
        if (pipeline)
            return pipeline->getSdlPipeline();
        return custom_pipeline;
    }

    /// @brief Sets custom vertex uniforms.
    /// @param data The uniform data struct.
    template<typename T>
    void
    setVertexUniforms(const T& data)
    {
        setVertexUniformsRaw(&data, sizeof(T));
    }

    /// @brief Sets custom fragment uniforms.
    /// @param data The uniform data struct.
    template<typename T>
    void
    setFragmentUniforms(const T& data)
    {
        setFragmentUniformsRaw(&data, sizeof(T));
    }

    /// @brief Default constructor.
    Material() = default;
    /// @brief Constructs a material with an albedo map.
    /// @param texture Pointer to the texture.
    explicit Material(Texture* texture)
      : albedoMap(texture)
      , custom_pipeline(nullptr)
      , pipeline(nullptr)
    {
    }
    /// @brief Constructs a material with an albedo map and custom pipeline.
    /// @param texture Pointer to the texture.
    /// @param pipeline Pointer to the custom pipeline.
    Material(Texture* texture, SDL_GPUGraphicsPipeline* pipeline)
      : albedoMap(texture)
      , custom_pipeline(pipeline)
      , pipeline(nullptr)
    {
    }
    /// @brief Constructs a material with an albedo map and
    /// MainGraphicsPipeline.
    /// @param texture Pointer to the texture.
    /// @param pipeline Pointer to the MainGraphicsPipeline.
    Material(Texture* texture, MainGraphicsPipeline* pipeline)
      : albedoMap(texture)
      , custom_pipeline(nullptr)
      , pipeline(pipeline)
    {
    }

private:
    void
    setVertexUniformsRaw(const void* data, size_t size);
    void
    setFragmentUniformsRaw(const void* data, size_t size);
};

} // namespace lili
