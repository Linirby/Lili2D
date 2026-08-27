#include "lili2d/core/window.hpp"

#include <SDL3/SDL_init.h>

#include <algorithm>
#include <stdexcept>

namespace lili {

static int window_count = 0;

Window::Window(const std::string& title, int width, int height) {
    if (window_count == 0) {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
            throw std::runtime_error(
                "Failed to initialize SDL: " + std::string(SDL_GetError())
            );
        }
    }
    window_count++;
    window = SDL_CreateWindow(title.c_str(), width, height, 0);
    if (!window) {
        throw std::runtime_error(
            "Failed to create SDL_Window: " + std::string(SDL_GetError())
        );
    }
}

Window::~Window() {
    if (window) {
        SDL_DestroyWindow(window);
        window_count--;
        if (window_count == 0) {
            SDL_Quit();
        }
    }
}

Window::Window(Window&& other) noexcept
    : resizable(other.resizable),
      borderless(other.borderless),
      fullscreen(other.fullscreen),
      logical_width(other.logical_width),
      logical_height(other.logical_height),
      use_logical_resolution(other.use_logical_resolution),
      window(other.window) {
    other.window = nullptr;
}

Window&
Window::operator=(Window&& other) noexcept {
    if (this != &other) {
        if (window) {
            SDL_DestroyWindow(window);
            window_count--;
            if (window_count == 0) SDL_Quit();
        }
        resizable = other.resizable;
        borderless = other.borderless;
        fullscreen = other.fullscreen;
        logical_width = other.logical_width;
        logical_height = other.logical_height;
        use_logical_resolution = other.use_logical_resolution;
        window = other.window;
        other.window = nullptr;
    }
    return *this;
}

void
Window::setTitle(const std::string& title) {
    if (!SDL_SetWindowTitle(window, title.c_str()))
        throw std::runtime_error(
            "Failed to change window name: " + std::string(SDL_GetError())
        );
    SDL_SyncWindow(window);
}

void
Window::setSize(int width, int height) {
    if (!SDL_SetWindowSize(window, width, height))
        throw std::runtime_error(
            "Failed to change window size: " + std::string(SDL_GetError())
        );
    SDL_SyncWindow(window);
}

void
Window::setResizable(bool activate) {
    if (!SDL_SetWindowResizable(window, activate))
        throw std::runtime_error(
            "Failed to change window to resizable: " +
            std::string(SDL_GetError())
        );
    resizable = activate;
    SDL_SyncWindow(window);
}

void
Window::setBorderless(bool activate) {
    if (!SDL_SetWindowBordered(window, !activate))
        throw std::runtime_error(
            "Failed to change window to borderless: " +
            std::string(SDL_GetError())
        );
    borderless = activate;
    SDL_SyncWindow(window);
}

void
Window::setFullscreen(bool activate) {
    if (!SDL_SetWindowFullscreen(window, activate))
        throw std::runtime_error(
            "Failed to change window to fullscreen mode: " +
            std::string(SDL_GetError())
        );
    fullscreen = activate;
    SDL_SyncWindow(window);
}

void
Window::setRelativeMouseMode(bool activate) {
    if (!SDL_SetWindowRelativeMouseMode(window, activate))
        throw std::runtime_error(
            "Failed to change window to relative mouse mode: " +
            std::string(SDL_GetError())
        );
    SDL_SyncWindow(window);
}

std::string
Window::getTitle() const {
    return std::string(SDL_GetWindowTitle(window));
}

int
Window::getWidth() const {
    int w = 0;
    if (!SDL_GetWindowSize(window, &w, nullptr))
        throw std::runtime_error(
            "Failed to get window size: " + std::string(SDL_GetError())
        );
    return w;
}

int
Window::getHeight() const {
    int h = 0;
    if (!SDL_GetWindowSize(window, nullptr, &h))
        throw std::runtime_error(
            "Failed to get window size: " + std::string(SDL_GetError())
        );
    return h;
}

Vec2
Window::getSize() const {
    int w, h = 0;
    if (!SDL_GetWindowSize(window, &w, &h))
        throw std::runtime_error(
            "Failed to get window size: " + std::string(SDL_GetError())
        );
    return {(float)w, (float)h};
}

void
Window::setLogicalResolution(int width, int height) {
    logical_width = width;
    logical_height = height;
    use_logical_resolution = (width > 0 && height > 0);
}

Vec2
Window::getLogicalResolution() const {
    if (use_logical_resolution)
        return {
            static_cast<float>(logical_width),
            static_cast<float>(logical_height)
        };
    return getSize();
}

Vec2
Window::toLogicalCoords(float screen_x, float screen_y) const {
    if (!use_logical_resolution) return {screen_x, screen_y};

    float physical_w = static_cast<float>(getWidth());
    float physical_h = static_cast<float>(getHeight());

    if (physical_w <= 0.0f || physical_h <= 0.0f) return {screen_x, screen_y};

    float scale = std::min(
        physical_w / static_cast<float>(logical_width),
        physical_h / static_cast<float>(logical_height)
    );

    float viewport_w = static_cast<float>(logical_width) * scale;
    float viewport_h = static_cast<float>(logical_height) * scale;
    float viewport_x = (physical_w - viewport_w) / 2.0f;
    float viewport_y = (physical_h - viewport_h) / 2.0f;

    float logical_x = (screen_x - viewport_x) / scale;
    float logical_y = (screen_y - viewport_y) / scale;

    return {logical_x, logical_y};
}

}  // namespace lili
