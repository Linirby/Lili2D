#pragma once

/// @file render.hpp
/// @brief Aggregates the rendering components of the Lili2D engine, including
/// shapes, sprites, models, and text.

#include "lili2d/render/scene/common/animation.hpp"  // IWYU pragma: keep
#include "lili2d/render/scene/common/animation_player.hpp"  // IWYU pragma: keep
#include "lili2d/render/scene/common/atlas_map.hpp"  // IWYU pragma: keep
#include "lili2d/render/scene/common/material.hpp"  // IWYU pragma: keep
#include "lili2d/render/scene/common/model.hpp"  // IWYU pragma: keep
#include "lili2d/render/scene/common/text.hpp"  // IWYU pragma: keep

#include "lili2d/render/scene/core2d/sprite.hpp"  // IWYU pragma: keep
#include "lili2d/render/scene/core2d/animated_sprite.hpp"  // IWYU pragma: keep
#include "lili2d/render/scene/core2d/sprite_batch.hpp"  // IWYU pragma: keep

#include "lili2d/render/scene/shapes/line.hpp"  // IWYU pragma: keep
#include "lili2d/render/scene/shapes/rect.hpp"  // IWYU pragma: keep
#include "lili2d/render/scene/shapes/circle.hpp"  // IWYU pragma: keep

#include "lili2d/render/renderer.hpp"  // IWYU pragma: keep

namespace lili {

/// @brief Represents an animation as a sequence of frames.
class Animation;
/// @brief A player for an animation.
class AnimationPlayer;
/// @brief Manage the creation and storage of animations.
class AnimationRegistry;
/// @brief Loads and slices a texture atlas into frames and animations.
class AtlasMap;
/// @brief Represents a single slice of a uv map within a texture atlas.
struct SliceUV;
/// @brief Properties defining a material's appearance.
struct MaterialProps;
/// @brief A material used for rendering objects.
struct Material;
/// @brief Manages the creation and storage of materials.
class MaterialRegistry;
/// @brief A 2D or 3D model containing vertex data.
struct Model;
/// @brief A font based on a bitmap texture.
class BitmapFont;
/// @brief Renderable text using a specific font.
class Text;

/// @brief A renderable 2D sprite.
class Sprite;
/// @brief A renderable animated 2D sprite driven by an Animation.
class AnimatedSprite;
/// @brief A renderable batch of 2D sprites.
class SpriteBatch;

/// @brief Defines the properties of a line shape.
struct LineShape;
/// @brief A renderable line object.
class Line;
/// @brief Defines the properties of a rectangle shape.
struct RectShape;
/// @brief A renderable rectangle object.
class Rect;
/// @brief Defines the properties of a circle shape.
struct CircleShape;
/// @brief A renderable circle object.
class Circle;

/// @brief The main renderer responsible for drawing objects to the screen.
class Renderer;

}  // namespace lili
