#pragma once

#include "lili2d/render/common/material.hpp"
#include "lili2d/render/gpu/gpu_mesh.hpp"

namespace lili {

struct BatchItem
{
    std::unique_ptr<GPUMesh> mesh;
    Material material;
};

} // namespace lili
