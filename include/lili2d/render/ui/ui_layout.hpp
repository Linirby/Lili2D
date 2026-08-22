#pragma once

#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/vec2.hpp"

namespace lili {

/// @brief Defines UI anchor alignment points relative to the viewport.
enum class Anchor {
    TOP_LEFT,      ///< Top-left corner of the viewport.
    TOP,           ///< Top-center of the viewport.
    TOP_RIGHT,     ///< Top-right corner of the viewport.
    LEFT,          ///< Middle-left of the viewport.
    CENTER,        ///< Center of the viewport.
    RIGHT,         ///< Middle-right of the viewport.
    BOTTOM_LEFT,   ///< Bottom-left corner of the viewport.
    BOTTOM,        ///< Bottom-center of the viewport.
    BOTTOM_RIGHT,  ///< Bottom-right corner of the viewport.
    CUSTOM         ///< Custom normalized anchor coordinates [0, 1].
};

/// @brief Defines UI pivot alignment points relative to the element's bounding
/// box.
enum class Pivot {
    TOP_LEFT,      ///< Top-left corner of the element.
    TOP,           ///< Top-center of the element.
    TOP_RIGHT,     ///< Top-right corner of the element.
    LEFT,          ///< Middle-left of the element.
    CENTER,        ///< Center of the element.
    RIGHT,         ///< Middle-right of the element.
    BOTTOM_LEFT,   ///< Bottom-left corner of the element.
    BOTTOM,        ///< Bottom-center of the element.
    BOTTOM_RIGHT,  ///< Bottom-right corner of the element.
    CUSTOM         ///< Custom normalized pivot coordinates [0, 1].
};

/// @brief Converts an Anchor enum value to a normalized 2D vector coordinate.
/// @param anchor Target anchor enum.
/// @param custom Custom anchor vector used when anchor is Anchor::CUSTOM.
/// @return Normalized 2D vector [0, 1].
Vec2
anchorToVector(Anchor anchor, Vec2 custom = {0.0f, 0.0f});

/// @brief Converts a Pivot enum value to a normalized 2D vector coordinate.
/// @param pivot Target pivot enum.
/// @param custom Custom pivot vector used when pivot is Pivot::CUSTOM.
/// @return Normalized 2D vector [0, 1].
Vec2
pivotToVector(Pivot pivot, Vec2 custom = {0.0f, 0.0f});

/// @brief Layout configuration for anchoring and positioning UI elements.
struct UILayout {
    Anchor anchor = Anchor::TOP_LEFT;  ///< Anchor alignment point.
    Pivot pivot = Pivot::TOP_LEFT;     ///< Pivot alignment point.
    Vec2 offset = {0.0f, 0.0f};        ///< Pixel offset from anchor.
    Vec2 custom_anchor = {
        0.0f, 0.0f
    };  ///< Custom anchor vector (when Anchor::CUSTOM).
    Vec2 custom_pivot = {
        0.0f, 0.0f
    };  ///< Custom pivot vector (when Pivot::CUSTOM).

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
