#pragma once

#include "lili2d/geometry/mat3x3.hpp"
#include "lili2d/geometry/vec2.hpp"
#include "lili2d/physics/aabb_collider.hpp"

namespace lili {

/// @brief Represents a 2D camera for scene viewing.
class Camera {
public:
    /// @brief Default constructor.
    Camera() noexcept = default;

    /// @brief Sets the camera's position.
    /// @param pos The new position.
    inline void
    setPosition(Vec2 pos) noexcept {
        position = pos;
    }

    /// @brief Sets the camera's rotation.
    /// @param degree The rotation in degrees.
    inline void
    setRotation(float degree) noexcept {
        rotation = degree;
    }

    /// @brief Sets the camera's zoom level.
    /// @param value The zoom level.
    inline void
    setZoom(float value) noexcept {
        zoom = value;
    }

    /// @brief Gets the camera's position.
    /// @return The current position.
    [[nodiscard]] inline Vec2
    getPosition() const noexcept {
        return position;
    }

    /// @brief Gets the camera's rotation.
    /// @return The rotation angle in degrees.
    [[nodiscard]] inline float
    getRotation() const noexcept {
        return rotation;
    }

    /// @brief Gets the camera's zoom level.
    /// @return The current zoom level.
    [[nodiscard]] inline float
    getZoom() const noexcept {
        return zoom;
    }

    /// @brief Gets the view matrix.
    /// @param width The viewport width.
    /// @param height The viewport height.
    /// @return The view transformation matrix.
    [[nodiscard]] Mat3
    getViewMatrix(float width, float height) const noexcept;
    /// @brief Gets the projection matrix.
    /// @param width The viewport width.
    /// @param height The viewport height.
    /// @return The projection matrix.
    [[nodiscard]] Mat3
    getProjection(float width, float height) const noexcept;

    /// @brief Gets the visible world-space bounds of the camera.
    /// @param width The viewport width.
    /// @param height The viewport height.
    /// @return The visible world-space bounds as an AABB2.
    [[nodiscard]] AABB2
    getViewportBounds(float width, float height) const noexcept;

private:
    Vec2 position = {};
    float rotation = 0.0f;
    float zoom = 1.0f;
};

}  // namespace lili
