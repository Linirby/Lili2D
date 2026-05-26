#pragma once

#include "render/core/gpu_mesh.hpp"

namespace lili {

/**
 * \brief Creates a unit quad mesh.
 * \return The mesh data.
 */
MeshData create_unit_quad();
/**
 * \brief Creates a unit circle mesh.
 * \param segments The number of segments.
 * \return The mesh data.
 */
MeshData create_unit_circle(int segments);

}  // namespace lili
