#pragma once

#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/vec2.hpp"

namespace lili {

enum class Anchor {
    TopLeft,
    Top,
    TopRight,
    Left,
    Center,
    Right,
    BottomLeft,
    Bottom,
    BottomRight,
    Custom
};

enum class Pivot {
    TopLeft,
    Top,
    TopRight,
    Left,
    Center,
    Right,
    BottomLeft,
    Bottom,
    BottomRight,
    Custom
};

Vec2
anchorToVector(Anchor anchor, Vec2 custom = {0.0f, 0.0f});
Vec2
pivotToVector(Pivot pivot, Vec2 custom = {0.0f, 0.0f});

struct UILayout {
    Anchor anchor = Anchor::TopLeft;
    Pivot pivot = Pivot::TopLeft;
    Vec2 offset = {0.0f, 0.0f};
    Vec2 custom_anchor = {0.0f, 0.0f};
    Vec2 custom_pivot = {0.0f, 0.0f};

    Vec2
    getScreenPosition(Vec2 viewport_size, Vec2 obj_size) const;
    Mat3
    getTransformationMatrix(
        Vec2 viewport_size, Vec2 obj_size, float rotation_rad = 0.0f,
        Vec2 scale = {1.0f, 1.0f}
    ) const;
};

}  // namespace lili
