#pragma once

#include "../../core/gpu_mesh.hpp"

namespace lili {

/// @brief Creates a unit quad mesh.
/// @return The mesh data.
MeshData createUnitQuad();
/// @brief Creates a unit circle mesh.
/// @param segments The number of segments.
/// @return The mesh data.
MeshData createUnitCircle(int segments);

}  // namespace lili
