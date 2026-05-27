#pragma once

#include "render/core/gpu_mesh.hpp"
#include "render/scene/material.hpp"

namespace lili {

/**
 * \brief Associates a GPU mesh with a material for rendering.
 */
struct Model {
	GPUMesh *mesh = nullptr;  ///< Pointer to the GPU mesh.
	Material *material = nullptr;  ///< Pointer to the material.

	/// \brief Default constructor.
	Model();
	/**
	 * \brief Constructs a model with a mesh and material.
	 * \param mesh Pointer to the GPU mesh.
	 * \param material Pointer to the material.
	 */
	Model(GPUMesh *mesh, Material *material);
};

}  // namespace lili
