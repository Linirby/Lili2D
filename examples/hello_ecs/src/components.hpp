#pragma once

#include <lili2d/geometry/vec2.hpp>
#include <lili2d/geometry/vec4.hpp>
#include <lili2d/render/scene/common/atlas_map.hpp>

struct PositionComponent {
    lili::Vec2 value;
};

struct VelocityComponent {
    lili::Vec2 value;
};

struct RenderComponent {
    lili::SliceUV slice;
    lili::Vec4 color;
    float radius;
};
