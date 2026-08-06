#pragma once

#include "SDL3/SDL_events.h"

using Keycode = SDL_Keycode;
using Scancode = SDL_Scancode;

namespace lili {

/// @brief Types of events that can occur.
enum class EventType {
    NONE,          ///< No event occurred.
    QUIT,          ///< The application was requested to quit.
    KEYBOARD,      ///< A keyboard key was pressed or released.
    MOUSE_BUTTON,  ///< A mouse button was pressed or released.
    MOUSE_MOTION,  ///< The mouse was moved.
    MOUSE_WHEEL,   ///< The mouse wheel was scrolled.
    WINDOW         ///< A window event occurred.
};

/// @brief Types of window events.
enum class WindowEventType {
    WINDOW_SHOWN,    ///< The window was shown.
    WINDOW_HIDDEN,   ///< The window was hidden.
    WINDOW_EXPOSED,  ///< The window was exposed and should be redrawn.
    WINDOW_MOVED,    ///< The window was moved.
    WINDOW_RESIZED,  ///< The window was resized.
    WINDOW_PIXEL_SIZE_CHANGED,  ///< The pixel size of the window has changed.
    WINDOW_METAL_VIEW_RESIZED,  ///< The pixel size of a Metal view has changed.
    WINDOW_MINIMIZED,           ///< The window was minimized.
    WINDOW_MAXIMIZED,           ///< The window was maximized.
    WINDOW_RESTORED,      ///< The window was restored to normal size/position.
    WINDOW_MOUSE_ENTER,   ///< The mouse entered the window.
    WINDOW_MOUSE_LEAVE,   ///< The mouse left the window.
    WINDOW_FOCUS_GAINED,  ///< The window gained keyboard focus.
    WINDOW_FOCUS_LOST,    ///< The window lost keyboard focus.
    WINDOW_CLOSE_REQUESTED,        ///< The window manager requested close.
    WINDOW_HIT_TEST,               ///< The window had a hit test.
    WINDOW_ICCPROF_CHANGED,        ///< The ICC profile has changed.
    WINDOW_DISPLAY_CHANGED,        ///< The window display has changed.
    WINDOW_DISPLAY_SCALE_CHANGED,  ///< The window display scale has changed.
    WINDOW_SAFE_AREA_CHANGED,      ///< The window safe area has changed.
    WINDOW_OCCLUDED,               ///< The window has been occluded.
    WINDOW_ENTER_FULLSCREEN,       ///< The window entered fullscreen.
    WINDOW_LEAVE_FULLSCREEN,       ///< The window left fullscreen.
    WINDOW_DESTROYED,         ///< The window is being or has been destroyed.
    WINDOW_HDR_STATE_CHANGED  ///< The window HDR state has changed.
};

/// @brief Contains data for a window event.
struct WindowEvent {
    WindowEventType type;  ///< The type of window event.
    int32_t data1;         ///< Event data 1 (e.g. width, x).
    int32_t data2;         ///< Event data 2 (e.g. height, y).
};

/// @brief Represents keyboard key action types.
enum class KeyAction {
    PRESSED,  ///< The key was pressed down.
    RELEASED  ///< The key was released.
};

/// @brief Represents mouse buttons.
enum class MouseButton {
    UNKNOWN,  ///< An unknown mouse button.
    LEFT,     ///< The left mouse button.
    MIDDLE,   ///< The middle mouse button (scroll wheel click).
    RIGHT     ///< The right mouse button.
};

/// @brief Represents mouse button action types.
enum class MouseAction {
    PRESSED,  ///< The mouse button was pressed down.
    RELEASED  ///< The mouse button was released.
};

/// @brief Contains data for a keyboard event.
struct KeyboardEvent {
    Keycode key;        ///< The keycode of the key.
    Scancode scancode;  ///< The scancode of the key.
    KeyAction action;   ///< The action performed on the key.
    bool repeat;        ///< True if this is a key repeat event.
};

/// @brief Contains data for a mouse button event.
struct MouseButtonEvent {
    MouseButton button;  ///< The mouse button involved.
    MouseAction action;  ///< The action performed on the mouse button.
    float x;             ///< The X coordinate of the mouse.
    float y;             ///< The Y coordinate of the mouse.
};

/// @brief Contains data for a mouse motion event.
struct MouseMotionEvent {
    float x;   ///< The X coordinate of the mouse.
    float y;   ///< The Y coordinate of the mouse.
    float dx;  ///< The change in X since the last event.
    float dy;  ///< The change in Y since the last event.
};

/// @brief Contains data for a mouse wheel event.
struct MouseWheelEvent {
    float dx;  ///< The horizontal scroll amount.
    float dy;  ///< The vertical scroll amount.
};

/// @brief Processes and provides access to system events.
class Event {
public:
    /// @brief Polls the next event from the queue.
    /// @return True if an event was processed, false if the queue is empty.
    bool
    poll();
    /// @brief Gets the type of the current event.
    /// @return The current event type.
    EventType
    type() const;
    /// @brief Checks if a specific key was just pressed based on an event.
    /// @param keyboard The keyboard event to check against.
    /// @return True if the key was just pressed, false otherwise.
    bool
    keyJustPressed(const KeyboardEvent& keyboard) const;
    /// @brief Checks if a specific mouse button was pressed based on an event.
    /// @param mouse The mouse button event to check against.
    /// @return True if the button was pressed, false otherwise.
    bool
    mouseButtonPressed(const MouseButtonEvent& mouse) const;

    /// @brief Gets the keyboard event data.
    /// @return The keyboard event data.
    const KeyboardEvent
    keyboard() const;
    /// @brief Gets the mouse button event data.
    /// @return The mouse button event data.
    const MouseButtonEvent
    mouseButton() const;
    /// @brief Gets the mouse motion event data.
    /// @return The mouse motion event data.
    const MouseMotionEvent
    mouseMotion() const;
    /// @brief Gets the mouse wheel event data.
    /// @return The mouse wheel event data.
    const MouseWheelEvent
    mouseWheel() const;
    /// @brief Gets the window event data.
    /// @return The window event data.
    const WindowEvent
    window() const;

private:
    /// @brief Internal SDL_Event structure.
    SDL_Event sdl_event = SDL_Event();
    /// @brief Cached event type.
    EventType current_type = EventType::NONE;

    /// @brief Resolves the engine EventType from the internal SDL_Event.
    /// @return Resolved EventType.
    EventType
    resolveType() const;
};

}  // namespace lili
