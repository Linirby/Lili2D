#include "lili2d/core/game_config.hpp"

#include "lili2d/core/game.hpp"
#include "lili2d/core/window.hpp"

namespace lili {

int
GameConfig::getWindowWidth() const {
    return window_w;
}

int
GameConfig::getWindowHeight() const {
    return window_h;
}

bool
GameConfig::isWindowFullscreen() const {
    return window_fullscreen;
}

bool
GameConfig::isWindowResizable() const {
    return window_resizable;
}

bool
GameConfig::isWindowBorderless() const {
    return window_borderless;
}

bool
GameConfig::isRelativeMouseMode() const {
    return relative_mouse_mode;
}

void
GameConfig::setWindowSize(lili::Game* game, int width, int height) {
    if (width > 0 && height > 0) {
        window_w = width;
        window_h = height;
        if (game && game->getWindow()) {
            game->getWindow()->setSize(width, height);
        }
    }
}

void
GameConfig::updateWindowSize(int width, int height) {
    if (width > 0 && height > 0) {
        window_w = width;
        window_h = height;
    }
}

void
GameConfig::setWindowFullscreen(lili::Game* game, bool set_fullscreen) {
    window_fullscreen = set_fullscreen;
    if (game && game->getWindow()) {
        lili::Window* win = game->getWindow();
        win->setFullscreen(set_fullscreen);
        window_w = win->getWidth();
        window_h = win->getHeight();
    }
}

void
GameConfig::updateWindowFullscreen(bool set_fullscreen) {
    window_fullscreen = set_fullscreen;
}

void
GameConfig::setWindowResizable(lili::Game* game, bool resizable) {
    window_resizable = resizable;
    if (game && game->getWindow()) {
        game->getWindow()->setResizable(resizable);
    }
}

void
GameConfig::updateWindowResizable(bool resizable) {
    window_resizable = resizable;
}

void
GameConfig::setWindowBorderless(lili::Game* game, bool borderless) {
    window_borderless = borderless;
    if (game && game->getWindow()) {
        game->getWindow()->setBorderless(borderless);
    }
}

void
GameConfig::updateWindowBorderless(bool borderless) {
    window_borderless = borderless;
}

void
GameConfig::setRelativeMouseMode(lili::Game* game, bool relative_mouse) {
    relative_mouse_mode = relative_mouse;
    if (game && game->getWindow()) {
        game->getWindow()->setRelativeMouseMode(relative_mouse);
    }
}

void
GameConfig::updateRelativeMouseMode(bool relative_mouse) {
    relative_mouse_mode = relative_mouse;
}

GameConfig&
GameConfig::get() {
    static GameConfig game_config;
    return game_config;
}

}  // namespace lili
