#include "lili2d/render/ui/ui_layout.hpp"

#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/vec2.hpp"

namespace lili {

Vec2
UILayout::getScreenPosition(Vec2 viewport_size, Vec2 obj_size) const noexcept {
    Vec2 anchor_vec = anchorToVector(anchor, custom_anchor);
    Vec2 pivot_vec = pivotToVector(pivot, custom_pivot);

    Vec2 anchor_point =
        Vec2(anchor_vec.x * viewport_size.x, anchor_vec.y * viewport_size.y);
    Vec2 pivot_point = Vec2(pivot_vec.x * obj_size.x, pivot_vec.y * obj_size.y);

    return anchor_point + offset - pivot_point;
}

Mat3
UILayout::getTransformationMatrix(
    Vec2 viewport_size, Vec2 obj_size, float rotation_rad, Vec2 scale
) const noexcept {
    Vec2 anchor_vec = anchorToVector(anchor, custom_anchor);
    Vec2 pivot_vec = pivotToVector(pivot, custom_pivot);

    Vec2 anchor_point =
        Vec2(anchor_vec.x * viewport_size.x, anchor_vec.y * viewport_size.y);
    Vec2 pivot_point = Vec2(pivot_vec.x * obj_size.x, pivot_vec.y * obj_size.y);
    Vec2 pivot_screen = anchor_point + offset;

    return Mat3::translate(pivot_screen) * Mat3::rotation(rotation_rad) *
           Mat3::translate({-pivot_point.x, -pivot_point.y}) *
           Mat3::scale(scale);
}

}  // namespace lili
