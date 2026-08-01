#include "lili2d/render/ui/ui_layout.hpp"

#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/vec2.hpp"

namespace lili {

Vec2
anchorToVector(Anchor anchor, Vec2 custom) {
    switch (anchor) {
        case Anchor::TopLeft:
            return Vec2(0.0f, 0.0f);
        case Anchor::Top:
            return Vec2(0.5f, 0.0f);
        case Anchor::TopRight:
            return Vec2(1.0f, 0.0f);
        case Anchor::Left:
            return Vec2(0.0f, 0.5f);
        case Anchor::Center:
            return Vec2(0.5f, 0.5f);
        case Anchor::Right:
            return Vec2(1.0f, 0.5f);
        case Anchor::BottomLeft:
            return Vec2(0.0f, 1.0f);
        case Anchor::Bottom:
            return Vec2(0.5f, 1.0f);
        case Anchor::BottomRight:
            return Vec2(1.0f, 1.0f);
        case Anchor::Custom:
            return custom;
    }
    return Vec2(0.0f, 0.0f);
}

Vec2
pivotToVector(Pivot pivot, Vec2 custom) {
    return anchorToVector(static_cast<Anchor>(pivot), custom);
}

Vec2
UILayout::getScreenPosition(Vec2 viewport_size, Vec2 obj_size) const {
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
) const {
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
