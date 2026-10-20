#pragma once

#include "lili2d/geometry/vec2.hpp"
#include <cstdint>
#include <type_traits>

namespace lili {

/// @brief Defines UI anchor alignment points relative to the viewport.
enum class Anchor : uint8_t
{
    TOP_LEFT,     ///< Top-left corner of the viewport.
    TOP,          ///< Top-center of the viewport.
    TOP_RIGHT,    ///< Top-right corner of the viewport.
    LEFT,         ///< Middle-left of the viewport.
    CENTER,       ///< Center of the viewport.
    RIGHT,        ///< Middle-right of the viewport.
    BOTTOM_LEFT,  ///< Bottom-left corner of the viewport.
    BOTTOM,       ///< Bottom-center of the viewport.
    BOTTOM_RIGHT, ///< Bottom-right corner of the viewport.
    CUSTOM        ///< Custom normalized anchor coordinates [0, 1].
};

/// @brief Defines UI pivot alignment points relative to the element's bounding
/// box.
enum class Pivot : uint8_t
{
    TOP_LEFT,     ///< Top-left corner of the element.
    TOP,          ///< Top-center of the element.
    TOP_RIGHT,    ///< Top-right corner of the element.
    LEFT,         ///< Middle-left of the element.
    CENTER,       ///< Center of the element.
    RIGHT,        ///< Middle-right of the element.
    BOTTOM_LEFT,  ///< Bottom-left corner of the element.
    BOTTOM,       ///< Bottom-center of the element.
    BOTTOM_RIGHT, ///< Bottom-right corner of the element.
    CUSTOM        ///< Custom normalized pivot coordinates [0, 1].
};

/// @brief Component for anchoring and aligning an entity in viewport/UI space.
struct UILayoutComponent
{
    Vec2 size = { 0.0f, 0.0f };          ///< Optional size override (if {0,0}, uses entity's shape/render2d size).
    Vec2 offset = { 0.0f, 0.0f };        ///< Pixel offset from anchor point.
    Vec2 custom_anchor = { 0.0f, 0.0f }; ///< Normalized custom anchor [0, 1] when anchor == Anchor::CUSTOM.
    Vec2 custom_pivot = { 0.0f, 0.0f };  ///< Normalized custom pivot [0, 1] when pivot == Pivot::CUSTOM.
    Anchor anchor = Anchor::CENTER;      ///< Viewport anchor alignment.
    Pivot pivot = Pivot::CENTER;         ///< Element pivot alignment.
};

static_assert(
    std::is_trivially_copyable_v<UILayoutComponent>,
    "UILayoutComponent must be trivially copyable"
);

} // namespace lili
