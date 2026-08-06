#pragma once

#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/vec2.hpp"

namespace lili {

/// @brief Defines UI anchor alignment points relative to the viewport.
enum class Anchor {
    TopLeft,      ///< Top-left corner of the viewport.
    Top,          ///< Top-center of the viewport.
    TopRight,     ///< Top-right corner of the viewport.
    Left,         ///< Middle-left of the viewport.
    Center,       ///< Center of the viewport.
    Right,        ///< Middle-right of the viewport.
    BottomLeft,   ///< Bottom-left corner of the viewport.
    Bottom,       ///< Bottom-center of the viewport.
    BottomRight,  ///< Bottom-right corner of the viewport.
    Custom        ///< Custom normalized anchor coordinates [0, 1].
};

/// @brief Defines UI pivot alignment points relative to the element's bounding
/// box.
enum class Pivot {
    TopLeft,      ///< Top-left corner of the element.
    Top,          ///< Top-center of the element.
    TopRight,     ///< Top-right corner of the element.
    Left,         ///< Middle-left of the element.
    Center,       ///< Center of the element.
    Right,        ///< Middle-right of the element.
    BottomLeft,   ///< Bottom-left corner of the element.
    Bottom,       ///< Bottom-center of the element.
    BottomRight,  ///< Bottom-right corner of the element.
    Custom        ///< Custom normalized pivot coordinates [0, 1].
};

/// @brief Converts an Anchor enum value to a normalized 2D vector coordinate.
/// @param anchor Target anchor enum.
/// @param custom Custom anchor vector used when anchor is Anchor::Custom.
/// @return Normalized 2D vector [0, 1].
Vec2
anchorToVector(Anchor anchor, Vec2 custom = {0.0f, 0.0f});

/// @brief Converts a Pivot enum value to a normalized 2D vector coordinate.
/// @param pivot Target pivot enum.
/// @param custom Custom pivot vector used when pivot is Pivot::Custom.
/// @return Normalized 2D vector [0, 1].
Vec2
pivotToVector(Pivot pivot, Vec2 custom = {0.0f, 0.0f});

/// @brief Layout configuration for anchoring and positioning UI elements.
struct UILayout {
    Anchor anchor = Anchor::TopLeft;  ///< Anchor alignment point.
    Pivot pivot = Pivot::TopLeft;     ///< Pivot alignment point.
    Vec2 offset = {0.0f, 0.0f};       ///< Pixel offset from anchor.
    Vec2 custom_anchor = {
        0.0f, 0.0f
    };  ///< Custom anchor vector (when Anchor::Custom).
    Vec2 custom_pivot = {
        0.0f, 0.0f
    };  ///< Custom pivot vector (when Pivot::Custom).

    /// @brief Calculates the screen position for an element given viewport and
    /// object sizes.
    /// @param viewport_size Size of the screen viewport in pixels.
    /// @param obj_size Size of the UI element in pixels.
    /// @return 2D screen position vector.
    Vec2
    getScreenPosition(Vec2 viewport_size, Vec2 obj_size) const;

    /// @brief Calculates the 3x3 transformation matrix for UI rendering.
    /// @param viewport_size Size of the screen viewport in pixels.
    /// @param obj_size Size of the UI element in pixels.
    /// @param rotation_rad Rotation angle in radians (default: 0).
    /// @param scale 2D scale vector (default: {1, 1}).
    /// @return 3x3 transformation matrix.
    Mat3
    getTransformationMatrix(
        Vec2 viewport_size, Vec2 obj_size, float rotation_rad = 0.0f,
        Vec2 scale = {1.0f, 1.0f}
    ) const;
};

}  // namespace lili
