#include "lili2d/core/event.hpp"

namespace lili {

bool
Event::poll() {
    if (SDL_PollEvent(&sdl_event)) {
        current_type = resolveType();
        return true;
    }
    return false;
}

EventType
Event::type() const {
    return current_type;
}

bool
Event::keyJustPressed(const KeyboardEvent& keyboard) const {
    return keyboard.action == KeyAction::PRESSED && !keyboard.repeat;
}

bool
Event::mouseButtonPressed(const MouseButtonEvent& mouse_button) const {
    return mouse_button.action == MouseAction::PRESSED;
}

const KeyboardEvent
Event::keyboard() const {
    return {
        .key = sdl_event.key.key,
        .scancode = sdl_event.key.scancode,
        .action = (sdl_event.type == SDL_EVENT_KEY_DOWN) ? KeyAction::PRESSED
                                                         : KeyAction::RELEASED,
        .repeat = sdl_event.key.repeat
    };
}

const MouseButtonEvent
Event::mouseButton() const {
    MouseButton button;
    switch (sdl_event.button.button) {
        case SDL_BUTTON_LEFT:
            button = MouseButton::LEFT;
            break;
        case SDL_BUTTON_MIDDLE:
            button = MouseButton::MIDDLE;
            break;
        case SDL_BUTTON_RIGHT:
            button = MouseButton::RIGHT;
            break;
        default:
            button = MouseButton::UNKNOWN;
            break;
    }
    return {
        .button = button,
        .action = (sdl_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                      ? MouseAction::PRESSED
                      : MouseAction::RELEASED,
        .x = sdl_event.button.x,
        .y = sdl_event.button.y
    };
}

const MouseMotionEvent
Event::mouseMotion() const {
    return {
        .x = sdl_event.motion.x,
        .y = sdl_event.motion.y,
        .dx = sdl_event.motion.xrel,
        .dy = sdl_event.motion.yrel
    };
}

const MouseWheelEvent
Event::mouseWheel() const {
    return {.dx = sdl_event.wheel.x, .dy = sdl_event.wheel.y};
}

const WindowEvent
Event::window() const {
    WindowEventType type;
    switch (sdl_event.type) {
        case SDL_EVENT_WINDOW_SHOWN:
            type = WindowEventType::WINDOW_SHOWN;
            break;
        case SDL_EVENT_WINDOW_HIDDEN:
            type = WindowEventType::WINDOW_HIDDEN;
            break;
        case SDL_EVENT_WINDOW_EXPOSED:
            type = WindowEventType::WINDOW_EXPOSED;
            break;
        case SDL_EVENT_WINDOW_MOVED:
            type = WindowEventType::WINDOW_MOVED;
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            type = WindowEventType::WINDOW_RESIZED;
            break;
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
            type = WindowEventType::WINDOW_PIXEL_SIZE_CHANGED;
            break;
        case SDL_EVENT_WINDOW_METAL_VIEW_RESIZED:
            type = WindowEventType::WINDOW_METAL_VIEW_RESIZED;
            break;
        case SDL_EVENT_WINDOW_MINIMIZED:
            type = WindowEventType::WINDOW_MINIMIZED;
            break;
        case SDL_EVENT_WINDOW_MAXIMIZED:
            type = WindowEventType::WINDOW_MAXIMIZED;
            break;
        case SDL_EVENT_WINDOW_RESTORED:
            type = WindowEventType::WINDOW_RESTORED;
            break;
        case SDL_EVENT_WINDOW_MOUSE_ENTER:
            type = WindowEventType::WINDOW_MOUSE_ENTER;
            break;
        case SDL_EVENT_WINDOW_MOUSE_LEAVE:
            type = WindowEventType::WINDOW_MOUSE_LEAVE;
            break;
        case SDL_EVENT_WINDOW_FOCUS_GAINED:
            type = WindowEventType::WINDOW_FOCUS_GAINED;
            break;
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            type = WindowEventType::WINDOW_FOCUS_LOST;
            break;
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            type = WindowEventType::WINDOW_CLOSE_REQUESTED;
            break;
        case SDL_EVENT_WINDOW_HIT_TEST:
            type = WindowEventType::WINDOW_HIT_TEST;
            break;
        case SDL_EVENT_WINDOW_ICCPROF_CHANGED:
            type = WindowEventType::WINDOW_ICCPROF_CHANGED;
            break;
        case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
            type = WindowEventType::WINDOW_DISPLAY_CHANGED;
            break;
        case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
            type = WindowEventType::WINDOW_DISPLAY_SCALE_CHANGED;
            break;
        case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED:
            type = WindowEventType::WINDOW_SAFE_AREA_CHANGED;
            break;
        case SDL_EVENT_WINDOW_OCCLUDED:
            type = WindowEventType::WINDOW_OCCLUDED;
            break;
        case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
            type = WindowEventType::WINDOW_ENTER_FULLSCREEN;
            break;
        case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
            type = WindowEventType::WINDOW_LEAVE_FULLSCREEN;
            break;
        case SDL_EVENT_WINDOW_DESTROYED:
            type = WindowEventType::WINDOW_DESTROYED;
            break;
        case SDL_EVENT_WINDOW_HDR_STATE_CHANGED:
            type = WindowEventType::WINDOW_HDR_STATE_CHANGED;
            break;
        default:
            type = WindowEventType::WINDOW_RESIZED;
            break;
    }

    return {
        .type = type,
        .data1 = static_cast<int32_t>(sdl_event.window.data1),
        .data2 = static_cast<int32_t>(sdl_event.window.data2)
    };
}

EventType
Event::resolveType() const {
    if (sdl_event.type >= SDL_EVENT_WINDOW_FIRST &&
        sdl_event.type <= SDL_EVENT_WINDOW_LAST) {
        return EventType::WINDOW;
    }

    switch (sdl_event.type) {
        case SDL_EVENT_QUIT:
            return EventType::QUIT;

        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
            return EventType::KEYBOARD;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP:
            return EventType::MOUSE_BUTTON;

        case SDL_EVENT_MOUSE_MOTION:
            return EventType::MOUSE_MOTION;
        case SDL_EVENT_MOUSE_WHEEL:
            return EventType::MOUSE_WHEEL;
        default:
            return EventType::NONE;
    }
}

}  // namespace lili
