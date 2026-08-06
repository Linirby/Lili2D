#pragma once

#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/vec2.hpp"

namespace lili {

/// @brief Forward declaration of AABB2.
struct AABB2;

/// @brief Represents a 2D camera for scene viewing.
class Camera {
public:
    /// @brief Sets the camera's position.
    /// @param pos The new position.
    void
    setPosition(const Vec2& pos);
    /// @brief Sets the camera's rotation.
    /// @param degree The rotation in degrees.
    void
    setRotation(float degree);
    /// @brief Sets the camera's zoom level.
    /// @param value The zoom level.
    void
    setZoom(float value);

    /// @brief Gets the camera's position.
    /// @return The current position.
    Vec2
    getPosition() const;
    /// @brief Gets the camera's zoom level.
    /// @return The current zoom level.
    float
    getZoom() const;
    /// @brief Gets the view matrix.
    /// @param width The viewport width.
    /// @param height The viewport height.
    /// @return The view transformation matrix.
    Mat3
    getViewMatrix(float width, float height) const;
    /// @brief Gets the projection matrix.
    /// @param width The viewport width.
    /// @param height The viewport height.
    /// @return The projection matrix.
    Mat3
    getProjection(float width, float height) const;

    /// @brief Gets the visible world-space bounds of the camera.
    /// @param width The viewport width.
    /// @param height The viewport height.
    /// @return The visible world-space bounds as an AABB2.
    AABB2
    getViewportBounds(float width, float height) const;

private:
    /// @brief Camera world position.
    Vec2 position;
    /// @brief Camera rotation angle in degrees.
    float rotation = 0.0f;
    /// @brief Camera zoom level multiplier.
    float zoom = 1.0f;

    /// @brief Flag indicating if cached view matrix needs recomputation.
    mutable bool is_dirty = true;
    /// @brief Cached viewport width.
    mutable float cached_width = 0.0f;
    /// @brief Cached viewport height.
    mutable float cached_height = 0.0f;
    /// @brief Cached 3x3 view matrix.
    mutable Mat3 cached_view_matrix;
};

}  // namespace lili
