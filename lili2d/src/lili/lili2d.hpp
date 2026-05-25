#pragma once

#include "lili/core.hpp"  // IWYU pragma: keep
#include "lili/geometry.hpp"  // IWYU pragma: keep
#include "lili/physics.hpp"  // IWYU pragma: keep
#include "lili/render.hpp"  // IWYU pragma: keep

namespace lili {

//
//  CORE
//
class Clock;

enum class EventType;
enum class KeyAction;
enum class MouseButton;
enum class MouseAction;
struct KeyboardEvent;
struct MouseButtonEvent;
struct MouseMotionEvent;
struct MouseWheelEvent;
class Event;

class Keyboard;

enum class MouseButton;
class Mouse;

struct SDLSystem;

class Window;

//
//  GEOMETRY
//
struct Mat3;
struct Mat4;

inline float deg_to_rad(float degrees);

struct Vec2;
struct Vec3;
struct Vec4;

//
//  PHYSICS
//
struct AABB;
struct RaycastResult;

//
//  RENDER
//
class Camera;

struct MaterialProps;
struct Material;
class MaterialRegistry;

struct Model;

struct RectShape;
class Rect;

class Sprite;

class BitmapFont;
class UIText;

class Renderer;

}  // namespace lili
