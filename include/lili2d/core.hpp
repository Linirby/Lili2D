#pragma once

/// @file core.hpp
/// @brief Aggregates the core components of the Lili2D engine, including window
/// management, input handling, and core events.

#include "lili2d/core/clock.hpp"  // IWYU pragma: keep
#include "lili2d/core/event.hpp"  // IWYU pragma: keep
#include "lili2d/core/keyboard.hpp"  // IWYU pragma: keep
#include "lili2d/core/mouse.hpp"  // IWYU pragma: keep
#include "lili2d/core/window.hpp"  // IWYU pragma: keep
#include "lili2d/core/game.hpp"  // IWYU pragma: keep
#include "lili2d/core/thread_pool.hpp"  // IWYU pragma: keep

namespace lili {

/// @brief A utility class for measuring and managing time.
class Clock;

/// @brief Types of events that can occur in the system.
enum class EventType;
/// @brief Keyboard key actions (e.g., press, release).
enum class KeyAction;
/// @brief Mouse buttons (e.g., left, right, middle).
enum class MouseButton;
/// @brief Mouse actions (e.g., press, release, motion).
enum class MouseAction;

/// @brief Event data for keyboard interactions.
struct KeyboardEvent;

/// @brief Event data for mouse button interactions.
struct MouseButtonEvent;
/// @brief Event data for mouse motion.
struct MouseMotionEvent;
/// @brief Event data for mouse wheel scrolling.
struct MouseWheelEvent;

/// @brief Base class or container for system events.
class Event;

/// @brief Handles keyboard input state and queries.
class Keyboard;

/// @brief Handles mouse input state and queries.
class Mouse;

/// @brief Represents an operating system window.
class Window;

/// @brief Base class for managing the game lifecycle and main loop.
class Game;

/// @brief A modern C++20 Thread Pool and Task Scheduler.
class ThreadPool;

}  // namespace lili
