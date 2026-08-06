#include "lili2d/render/interfaces/renderable.hpp"

#include "lili2d/render/renderer.hpp"

namespace lili {

void
IRenderable::setVisible(bool visible) {
    is_visible = visible;
}

bool
IRenderable::isVisible() const {
    return is_visible;
}

void
IRenderable::setUILayout(const UILayout& layout) {
    ui_layout = layout;
}

UILayout
IRenderable::getUILayout() const {
    return ui_layout;
}

void
IRenderable::setAnchor(Anchor anchor) {
    ui_layout.anchor = anchor;
}

Anchor
IRenderable::getAnchor() const {
    return ui_layout.anchor;
}

void
IRenderable::setPivot(Pivot pivot) {
    ui_layout.pivot = pivot;
}

Pivot
IRenderable::getPivot() const {
    return ui_layout.pivot;
}

void
IRenderable::setOffset(Vec2 offset) {
    ui_layout.offset = offset;
    setPosition(offset);
}

Vec2
IRenderable::getOffset() const {
    return ui_layout.offset;
}

Vec2
IRenderable::getGlobalPosition(const Renderer* renderer) const {
    if (getRender() == RenderLayer::UI && renderer) {
        Vec2 viewport_size = renderer->getLogicalResolution();
        return ui_layout.getScreenPosition(viewport_size, getSize());
    }
    return getPosition();
}

RectShape
IRenderable::getGlobalBounds(const Renderer* renderer) const {
    Vec2 pos = getGlobalPosition(renderer);
    Vec2 size = getSize();
    return RectShape(pos.x, pos.y, size.x, size.y);
}

bool
IRenderable::containsPoint(Vec2 point, const Renderer* renderer) const {
    if (!is_visible) return false;

    (void)renderer;
    Mat3 inv_mat = getTransformMatrix().inverse();
    Vec2 local_pt = inv_mat.transformPoint(point);
    Vec2 size = getSize();

    return local_pt.x >= 0.0f && local_pt.x <= size.x && local_pt.y >= 0.0f &&
           local_pt.y <= size.y;
}

}  // namespace lili
