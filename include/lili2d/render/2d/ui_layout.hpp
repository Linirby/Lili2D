#pragma once

#include "lili2d/components/ui_layout.hpp"
#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/vec2.hpp"

namespace lili {

/// @brief Converts an Anchor enum value to a normalized 2D vector coordinate.
/// @param anchor Target anchor enum.
/// @param custom Custom anchor vector used when anchor is Anchor::CUSTOM.
/// @return Normalized 2D vector [0, 1].
[[deprecated("Replaced by internal method in UILayoutSystem")]]
[[nodiscard]] constexpr inline Vec2
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

/// @brief Converts a Pivot enum value to a normalized 2D vector coordinate.
/// @param pivot Target pivot enum.
/// @param custom Custom pivot vector used when pivot is Pivot::CUSTOM.
/// @return Normalized 2D vector [0, 1].
[[deprecated("Replaced by internal method in UILayoutSystem")]]
[[nodiscard]] constexpr inline Vec2
pivotToVector(Pivot pivot, Vec2 custom = {}) noexcept
{
    return anchorToVector(static_cast<Anchor>(pivot), custom);
}

/// @brief Layout configuration for anchoring and positioning UI elements.
struct [[deprecated("Use UILayoutComponent with ECS")]] UILayout
{
    Anchor anchor = Anchor::CENTER; ///< Anchor alignment point.
    Pivot pivot = Pivot::CENTER;    ///< Pivot alignment point.
    Vec2 offset = {};               ///< Pixel offset from anchor.
    Vec2 custom_anchor = {};        ///< Custom anchor (when Anchor::CUSTOM).
    Vec2 custom_pivot = {};         ///< Custom pivot (when Pivot::CUSTOM).

    constexpr UILayout() noexcept = default;

    /// @brief Calculates the screen position for an element given viewport and
    /// object sizes.
    /// @param viewport_size Size of the screen viewport in pixels.
    /// @param obj_size Size of the UI element in pixels.
    /// @return 2D screen position vector.
    [[nodiscard]] Vec2
    getScreenPosition(Vec2 viewport_size, Vec2 obj_size) const noexcept;

    /// @brief Calculates the 3x3 transformation matrix for UI rendering.
    /// @param viewport_size Size of the screen viewport in pixels.
    /// @param obj_size Size of the UI element in pixels.
    /// @param rotation_rad Rotation angle in radians (default: 0).
    /// @param scale 2D scale vector (default: {1, 1}).
    /// @return 3x3 transformation matrix.
    [[nodiscard]] Mat3
    getTransformationMatrix(
        Vec2 viewport_size,
        Vec2 obj_size,
        float rotation_rad = 0.0f,
        Vec2 scale = { 1.0f, 1.0f }
    ) const noexcept;
};

} // namespace lili
