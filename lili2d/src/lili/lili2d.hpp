#pragma once

/// \file lili2d.hpp
/// \brief Main include header for the Lili2D engine. Aggregates all major
/// engine subsystems and forward declarations.

#include "lili/core.hpp"  // IWYU pragma: keep
#include "lili/geometry.hpp"  // IWYU pragma: keep
#include "lili/physics.hpp"  // IWYU pragma: keep
#include "lili/render.hpp"  // IWYU pragma: keep

namespace lili {

//
//  CORE
//
/// \name Core Components
/// \brief Core engine features including events, input handling, and windowing.
/// @{

/// \brief A utility class for measuring and managing time.
class Clock;

/// \brief Types of events that can occur in the system.
enum class EventType;
/// \brief Keyboard key actions (e.g., press, release).
enum class KeyAction;
/// \brief Mouse buttons (e.g., left, right, middle).
enum class MouseButton;
/// \brief Mouse actions (e.g., press, release, motion).
enum class MouseAction;

/// \brief Event data for keyboard interactions.
struct KeyboardEvent;
/// \brief Event data for mouse button interactions.
struct MouseButtonEvent;
/// \brief Event data for mouse motion.
struct MouseMotionEvent;
/// \brief Event data for mouse wheel scrolling.
struct MouseWheelEvent;

/// \brief Base class or container for system events.
class Event;

/// \brief Handles keyboard input state and queries.
class Keyboard;

/// \brief Handles mouse input state and queries.
class Mouse;


/// \brief Represents an operating system window.
class Window;

/// @}

//
//  GEOMETRY
//
/// \name Geometry Components
/// \brief Mathematical structures and geometric primitives.
/// @{

/// \brief A 3x3 matrix for 2D transformations.
struct Mat3;
/// \brief A 4x4 matrix for 3D or advanced transformations.
struct Mat4;

/// \brief Converts an angle from degrees to radians.
/// \param degrees The angle in degrees.
/// \return The angle in radians.
inline float deg_to_rad(float degrees);

/// \brief A 2D vector.
struct Vec2;
/// \brief A 3D vector.
struct Vec3;
/// \brief A 4D vector.
struct Vec4;

/// @}

//
//  PHYSICS
//
/// \name Physics Components
/// \brief Physics concepts such as collision detection and spatial queries.
/// @{

/// \brief An axis-aligned bounding box for collision detection.
struct AABB;
/// \brief Result data from a raycast operation.
struct RaycastResult;

/// @}

//
//  RENDER
//
/// \name Render Components
/// \brief Graphics rendering, scene management, and drawables.
/// @{

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

/// \brief Represents a single animation frame within a texture atlas.
struct AnimationFrame;
/// \brief Defines an animation as a sliceable image.
class Animation;
/// \brief A renderable animated 2D sprite driven by an Animation.
class AnimatedSprite;

/// \brief A font based on a bitmap texture.
class BitmapFont;
/// \brief Renderable text using a specific font.
class Text;

/// \brief The main renderer responsible for drawing objects to the screen.
class Renderer;

/// @}

}  // namespace lili
