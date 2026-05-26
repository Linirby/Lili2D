#pragma once

/// \file render.hpp
/// \brief Aggregates the rendering components of the Lili2D engine, including shapes, sprites, models, and text.

#include "render/scene/shapes/line.hpp"  // IWYU pragma: keep
#include "render/scene/shapes/rect.hpp"  // IWYU pragma: keep
#include "render/scene/shapes/circle.hpp"  // IWYU pragma: keep
#include "render/scene/camera.hpp"  // IWYU pragma: keep
#include "render/scene/material.hpp"  // IWYU pragma: keep
#include "render/scene/model.hpp"  // IWYU pragma: keep
#include "render/scene/sprite.hpp"  // IWYU pragma: keep
#include "render/scene/text.hpp"  // IWYU pragma: keep
#include "render/renderer.hpp"  // IWYU pragma: keep

namespace lili {

/// \brief Defines the properties of a line shape.
struct LineShape;
/// \brief A renderable line object.
class Line;
/// \brief Defines the properties of a rectangle shape.
struct RectShape;
/// \brief A renderable rectangle object.
class Rect;
/// \brief Defines the properties of a circle shape.
struct CircleShape;
/// \brief A renderable circle object.
class Circle;

/// \brief Defines a camera for viewing the scene.
class Camera;

/// \brief Properties defining a material's appearance.
struct MaterialProps;
/// \brief A material used for rendering objects.
struct Material;
/// \brief Manages the creation and storage of materials.
class MaterialRegistry;

/// \brief A 2D or 3D model containing vertex data.
struct Model;

/// \brief A renderable 2D sprite.
class Sprite;

/// \brief A font based on a bitmap texture.
class BitmapFont;
/// \brief Renderable text using a specific font.
class Text;

/// \brief The main renderer responsible for drawing objects to the screen.
class Renderer;

}  // namespace lili
