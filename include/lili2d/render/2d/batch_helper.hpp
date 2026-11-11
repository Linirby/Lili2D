#pragma once

#include "lili2d/geometry/vec2.hpp"
#include "lili2d/render/2d/bitmap_font.hpp"
#include "lili2d/render/common/slice_uv.hpp"
#include "lili2d/render/gpu/gpu_mesh.hpp"

/// @brief Helper functions for appending 2D primitives and sprites to a CPU
/// MeshData buffer.
namespace lili::BatchHelper {

/// @brief Appends a textured sprite quad to the mesh data.
/// @param mesh_data The target CPU mesh data.
/// @param slice The UV slice and pixel dimensions of the sprite.
/// @param pos The world/screen position.
/// @param scale The X and Y scale factors.
/// @param rotation The rotation angle in radians.
/// @param tint The RGBA color tint.
void
appendSprite(
    MeshData& mesh_data,
    const SliceUV& slice,
    Vec2 pos,
    Vec2 scale,
    float rotation,
    Vec4 tint
);

/// @brief Appends a filled rectangle quad to the mesh data.
/// @param mesh_data The target CPU mesh data.
/// @param pos The top-left position of the rectangle.
/// @param size The unscaled width and height.
/// @param scale The X and Y scale factors.
/// @param rotation The rotation angle in radians.
/// @param color The RGBA fill color.
void
appendRect(
    MeshData& mesh_data,
    Vec2 pos,
    Vec2 size,
    Vec2 scale,
    float rotation,
    Vec4 color
);

/// @brief Appends a hollow (outline) rectangle to the mesh data.
/// @param mesh_data The target CPU mesh data.
/// @param pos The top-left position of the rectangle.
/// @param size The unscaled width and height.
/// @param scale The X and Y scale factors.
/// @param rotation The rotation angle in radians.
/// @param color The RGBA outline color.
/// @param thickness The outline thickness in pixels.
void
appendHollowRect(
    MeshData& mesh_data,
    Vec2 pos,
    Vec2 size,
    Vec2 scale,
    float rotation,
    Vec4 color,
    float thickness
);

/// @brief Appends a filled circle fan/polygon to the mesh data.
/// @param mesh_data The target CPU mesh data.
/// @param center The center position of the circle.
/// @param radius The radius of the circle.
/// @param segments The number of polygon segments used to approximate the
/// circle.
/// @param color The RGBA fill color.
void
appendCircle(
    MeshData& mesh_data,
    Vec2 center,
    float radius,
    float segments,
    Vec4 color
);

/// @brief Appends a hollow (ring) circle to the mesh data.
/// @param mesh_data The target CPU mesh data.
/// @param center The center position of the circle.
/// @param radius The outer radius of the circle.
/// @param segments The number of polygon segments used to approximate the
/// circle.
/// @param color The RGBA outline color.
/// @param thickness The ring thickness in pixels.
void
appendHollowCircle(
    MeshData& mesh_data,
    Vec2 center,
    float radius,
    float segments,
    Vec4 color,
    float thickness
);

/// @brief Appends a line segment quad to the mesh data.
/// @param mesh_data The target CPU mesh data.
/// @param start_pos The starting point of the line.
/// @param dxy The delta vector (end - start) of the line.
/// @param thickness The thickness of the line in pixels.
/// @param color The RGBA color of the line.
void
appendLine(
    MeshData& mesh_data,
    Vec2 start_pos,
    Vec2 dxy,
    float thickness,
    Vec4 color
);

/// @brief Appends bitmap text glyph quads to the mesh data.
/// @param mesh_data The target CPU mesh data.
/// @param font Pointer to the BitmapFont used for glyph metrics and UVs.
/// @param text The UTF-8/ASCII string view to render.
/// @param pos The top-left starting position of the text.
/// @param scale The X and Y scale factors.
/// @param rotation The rotation angle in radians.
/// @param spacing Additional horizontal spacing between glyphs.
/// @param color The RGBA color tint of the text.
void
appendText(
    MeshData& mesh_data,
    BitmapFont* font,
    std::string_view text,
    Vec2 pos,
    Vec2 scale,
    float rotation,
    float spacing,
    Vec4 color
);

} // namespace lili::BatchHelper
