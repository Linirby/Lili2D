#pragma once

#include "lili2d/geometry/vec2.hpp"
#include "lili2d/geometry/vec3.hpp"
#include "lili2d/geometry/vec4.hpp"
namespace lili {

/// @brief Represents a single vertex in a mesh.
struct Vertex
{
    float x = 0.0f;           ///< X position.
    float y = 0.0f;           ///< Y position.
    float z = 0.0f;           ///< Z position.
    float u = 0.0f;           ///< U texture coordinate.
    float v = 0.0f;           ///< V texture coordinate.
    float material_id = 0.0f; ///< Material ID.
    float r = 1.0f;           ///< Red color.
    float g = 1.0f;           ///< Green color.
    float b = 1.0f;           ///< Blue color.
    float a = 1.0f;           ///< Alpha color.

    /// @brief Default constructor.
    constexpr Vertex() noexcept = default;

    /// @brief Constructs a Vertex with position, UV, material ID, and RGBA
    /// color.
    /// @param x X position.
    /// @param y Y position.
    /// @param z Z position.
    /// @param u U texture coordinate.
    /// @param v V texture coordinate.
    /// @param material_id Material ID.
    /// @param r Red color component.
    /// @param g Green color component.
    /// @param b Blue color component.
    /// @param a Alpha color component.
    constexpr Vertex(
        float x,
        float y,
        float z,
        float u = 0.0f,
        float v = 0.0f,
        float material_id = 0.0f,
        float r = 1.0f,
        float g = 1.0f,
        float b = 1.0f,
        float a = 1.0f
    ) noexcept
      : x(x)
      , y(y)
      , z(z)
      , u(u)
      , v(v)
      , material_id(material_id)
      , r(r)
      , g(g)
      , b(b)
      , a(a)
    {
    }

    /// @brief Constructs a Vertex with position, UV, material ID, and RGBA
    /// color vectors.
    /// @param xyz 3D position vector.
    /// @param uv 2D UV texture coordinate vector.
    /// @param material_id Material ID.
    /// @param rgba 4D RGBA color tint vector.
    constexpr Vertex(
        Vec3 xyz,
        Vec2 uv = {},
        float material_id = 0.0f,
        Vec4 rgba = { 1.0f, 1.0f, 1.0f, 1.0f }
    ) noexcept
      : x(xyz.x)
      , y(xyz.y)
      , z(xyz.z)
      , u(uv.x)
      , v(uv.y)
      , material_id(material_id)
      , r(rgba.x)
      , g(rgba.y)
      , b(rgba.z)
      , a(rgba.w)
    {
    }
};

} // namespace lili
