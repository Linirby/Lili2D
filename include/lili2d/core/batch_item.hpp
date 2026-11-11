#pragma once

#include "lili2d/render/common/material.hpp"
#include "lili2d/render/gpu/gpu_mesh.hpp"

namespace lili {

/// @brief Represents a pooled GPU mesh and material pair for batched rendering.
struct BatchItem
{
    std::unique_ptr<GPUMesh> mesh; ///< Owned GPU mesh buffer.
    Material material;             ///< Material bound to this batch item.
};

} // namespace lili
