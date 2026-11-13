#pragma once

#include "lili2d/components/render2d.hpp"
#include "lili2d/components/transform.hpp"
#include "lili2d/components/ui_layout.hpp"
#include "lili2d/core/thread_pool.hpp"
#include "lili2d/ecs/ecs_registry.hpp"
#include "lili2d/ecs/system.hpp"
#include "lili2d/geometry/vec2.hpp"

namespace lili {

class BitmapFont;

/// @brief System that computes screen-space positions for entities with
/// cUILayout.
class UILayoutSystem : public System<cUILayout, cRender2D, cTransform>
{
public:
    /// @brief Constructs a UILayoutSystem.
    /// @param viewport_size The current logical viewport size.
    /// @param default_font Fallback font used when measuring cText
    /// bounds.
    explicit UILayoutSystem(
        Vec2 viewport_size = {},
        BitmapFont* default_font = nullptr
    ) noexcept
      : viewport_size(viewport_size)
      , default_font(default_font)
    {
    }

    /// @brief Sets the viewport size used for anchor calculations.
    /// @param size The viewport width and height.
    inline void
    setViewportSize(Vec2 size) noexcept
    {
        viewport_size = size;
    }

    /// @brief Sets the default font used when measuring cText bounds.
    /// @param font Pointer to the fallback BitmapFont.
    inline void
    setDefaultFont(BitmapFont* font) noexcept
    {
        default_font = font;
    }

    /// @brief Recomputes the screen position of a single UI entity if dirty.
    /// @param registry The ECS registry.
    /// @param entity The entity being updated.
    /// @param layout The entity's cUILayout.
    /// @param render2d The entity's cRender2D.
    /// @param trans The entity's cTransform.
    /// @param dt Delta time (unused).
    void
    updateEntity(
        ECSRegistry& registry,
        Entity entity,
        cUILayout& layout,
        cRender2D& render2d,
        cTransform& trans,
        float dt
    ) override;

    /// @brief Updates all entities with cUILayout, cRender2D,
    /// and cTransform.
    /// @param registry The ECS registry.
    /// @param viewport_size The current logical viewport dimensions.
    /// @param thread_pool Optional thread pool for parallel execution.
    /// @param default_font Optional fallback font for text measurement.
    static void
    update(
        ECSRegistry& registry,
        Vec2 viewport_size,
        ThreadPool* thread_pool = nullptr,
        BitmapFont* default_font = nullptr
    );

private:
    Vec2 viewport_size;
    BitmapFont* default_font = nullptr;

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
    getScreenPosition(cUILayout layout, Vec2 obj_size, Vec2 viewport_size);
};

} // namespace lili
