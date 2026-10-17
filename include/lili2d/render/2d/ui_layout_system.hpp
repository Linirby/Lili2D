#pragma once

#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/2d/ui_layout.hpp"

namespace lili {

class UILayoutSystem
{
public:
    static void
    update(ECSRegistry& registry, Vec2 viewport_size);

private:
    [[nodiscard]] static constexpr inline Vec2
    anchorToVector(Anchor anchor, Vec2 custom = {}) noexcept
    {
        switch (anchor) {
            case Anchor::TOP_LEFT:
                return Vec2(0.0f, 0.0f);
            case Anchor::TOP:
                return Vec2(0.5f, 0.0f);
            case Anchor::TOP_RIGHT:
                return Vec2(1.0f, 0.0f);
            case Anchor::LEFT:
                return Vec2(0.0f, 0.5f);
            case Anchor::CENTER:
                return Vec2(0.5f, 0.5f);
            case Anchor::RIGHT:
                return Vec2(1.0f, 0.5f);
            case Anchor::BOTTOM_LEFT:
                return Vec2(0.0f, 1.0f);
            case Anchor::BOTTOM:
                return Vec2(0.5f, 1.0f);
            case Anchor::BOTTOM_RIGHT:
                return Vec2(1.0f, 1.0f);
            case Anchor::CUSTOM:
                return custom;
        }
        return Vec2(0.0f, 0.0f);
    }

    [[nodiscard]] static constexpr inline Vec2
    pivotToVector(Pivot pivot, Vec2 custom = {}) noexcept
    {
        return anchorToVector(static_cast<Anchor>(pivot), custom);
    }

    [[nodiscard]] static Vec2
    getScreenPosition(
        UILayoutComponent layout,
        Vec2 obj_size,
        Vec2 viewport_size
    );
};

} // namespace lili
