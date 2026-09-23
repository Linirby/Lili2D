#include "lili2d/render/interfaces/renderable.hpp"

#include "lili2d/render/renderer.hpp"

namespace lili {

Vec2
IRenderable::getGlobalPosition(const Renderer* renderer) const {
    bool is_ui = render_layer == RenderLayer::UI ||
                 render_layer == RenderLayer::PIXEL_UI;
    if (is_ui && renderer) {
        Vec2 viewport_size = renderer->getLogicalResolution();
        return ui_layout.getScreenPosition(viewport_size, getSize());
    }
    return getPosition();
}

RectShape
IRenderable::getGlobalBounds(const Renderer* renderer) const {
    Vec2 pos = getGlobalPosition(renderer);
    Vec2 size = getSize();
    return RectShape(pos, size);
}

bool
IRenderable::containsPoint(
    Vec2 point, [[maybe_unused]] const Renderer* renderer
) const {
    if (!is_visible) return false;

    Mat3 inv_mat = getTransformMatrix().inverse();
    Vec2 local_pt = inv_mat.transformPoint(point);
    Vec2 size = getSize();

    return local_pt.x >= 0.0f && local_pt.x <= size.x && local_pt.y >= 0.0f &&
           local_pt.y <= size.y;
}

}  // namespace lili
