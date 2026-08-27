#pragma once

#include "lili2d/render/core/gpu_mesh.hpp"
#include "lili2d/render/scene/common/material.hpp"

namespace lili {

/// @brief Associates a GPU mesh with a material for rendering.
struct Model {
    GPUMesh* mesh = nullptr;       ///< Pointer to the GPU mesh.
    Material* material = nullptr;  ///< Pointer to the material.

    /// @brief Default constructor.
    constexpr Model() noexcept = default;
    /// @brief Constructs a model with a mesh and material.
    /// @param mesh Pointer to the GPU mesh.
    /// @param material Pointer to the material.
    constexpr Model(GPUMesh* mesh, Material* material) noexcept
        : mesh(mesh), material(material) {}
};

}  // namespace lili
