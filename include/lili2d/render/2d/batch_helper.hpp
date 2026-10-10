#pragma once

#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/common/atlas_map.hpp"
#include "lili2d/render/gpu/gpu_mesh.hpp"

namespace lili::BatchHelper {

void
appendSprite(
    MeshData& mesh_data,
    const SliceUV& slice,
    Vec2 pos,
    Vec2 scale,
    float rotation,
    Vec4 tint
);

void
appendRect(
    MeshData& mesh_data,
    Vec2 pos,
    Vec2 size,
    Vec2 scale,
    float rotation,
    Vec4 color
);

void
appendHollowRect(
    MeshData& mesh_data,
    Vec2 pos,
    Vec2 size,
    Vec2 scale,
    float rotation,
    Vec4 color,
    float thickness
);

void
appendCircle(
    MeshData& mesh_data,
    Vec2 center,
    float radius,
    float segments,
    Vec4 color
);

void
appendHollowCircle(
    MeshData& mesh_data,
    Vec2 center,
    float radius,
    float segments,
    Vec4 color,
    float thickness
);

void
appendLine(MeshData& mesh_data, Vec2 dxy, float thickness, Vec4 color);

} // namespace lili::BatchHelper
