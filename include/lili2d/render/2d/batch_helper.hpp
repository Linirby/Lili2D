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
    Vec2 scale = { 1.0f, 1.0f },
    float rotation = 0.0f,
    Vec4 tint = { 1.0f, 1.0f, 1.0f, 1.0f }
);

void
appendRect(
    MeshData& mesh_data,
    Vec2 pos,
    Vec2 size,
    float rotation = 0.0f,
    Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }
);

void
appendHollowRect(
    MeshData& mesh_data,
    Vec2 pos,
    Vec2 size,
    float thickness,
    float rotation = 0.0f,
    Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }
);

void
appendCircle(
    MeshData& mesh_data,
    Vec2 center,
    float radius,
    float segments = 24,
    Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }
);

void
appendHollowCircle(
    MeshData& mesh_data,
    Vec2 center,
    float radius,
    float thickness,
    float segments = 24,
    Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }
);

void
appendLine(
    MeshData& mesh_data,
    Vec2 dxy,
    float thickness = 1.0f,
    Vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f }
);

} // namespace lili::BatchHelper
