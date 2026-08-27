#include "lili2d/world/camera.hpp"

#include <algorithm>

#include "lili2d/geometry/utils.hpp"
#include "lili2d/physics/aabb_collider.hpp"

namespace lili {

Mat3
Camera::getViewMatrix(float width, float height) const noexcept {
    Vec2 center(width * 0.5f, height * 0.5f);
    return Mat3::translate(center) * Mat3::rotation(degToRad(rotation)) *
           Mat3::scale(Vec2(zoom, zoom)) * Mat3::translate(-position);
}

Mat3
Camera::getProjection(float width, float height) const noexcept {
    return Mat3::orthographic(0.0f, width, height, 0.0f);
}

AABB2
Camera::getViewportBounds(float width, float height) const noexcept {
    Mat3 view = getViewMatrix(width, height);
    Mat3 inv_view = view.inverse();

    Vec2 p0 = inv_view.transformPoint(Vec2(0.0f, 0.0f));
    Vec2 p1 = inv_view.transformPoint(Vec2(width, 0.0f));
    Vec2 p2 = inv_view.transformPoint(Vec2(width, height));
    Vec2 p3 = inv_view.transformPoint(Vec2(0.0f, height));

    float min_x = std::min({p0.x, p1.x, p2.x, p3.x});
    float max_x = std::max({p0.x, p1.x, p2.x, p3.x});
    float min_y = std::min({p0.y, p1.y, p2.y, p3.y});
    float max_y = std::max({p0.y, p1.y, p2.y, p3.y});

    return AABB2(Vec2(min_x, min_y), Vec2(max_x - min_x, max_y - min_y));
}

}  // namespace lili
