#pragma once

#include "core/clock.hpp"  // IWYU pragma: keep
#include "core/event.hpp"  // IWYU pragma: keep
#include "core/keyboard.hpp"  // IWYU pragma: keep
#include "core/mouse.hpp"  // IWYU pragma: keep
#include "core/sdl_system.hpp"  // IWYU pragma: keep
#include "core/window.hpp"  // IWYU pragma: keep

namespace lili {

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

}  // namespace lili
