#include "lili2d/core/game_config.hpp"

#include "lili2d/core/game.hpp"
#include "lili2d/core/window.hpp"

namespace lili {

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
GameConfig::setWindowResizable(lili::Game* game, bool resizable) {
    window_resizable = resizable;
    if (game && game->getWindow()) {
        game->getWindow()->setResizable(resizable);
    }
}

void
GameConfig::setWindowBorderless(lili::Game* game, bool borderless) {
    window_borderless = borderless;
    if (game && game->getWindow()) {
        game->getWindow()->setBorderless(borderless);
    }
}

void
GameConfig::setRelativeMouseMode(lili::Game* game, bool relative_mouse) {
    relative_mouse_mode = relative_mouse;
    if (game && game->getWindow()) {
        game->getWindow()->setRelativeMouseMode(relative_mouse);
    }
}

GameConfig&
GameConfig::get() {
    static GameConfig instance;
    return instance;
}

}  // namespace lili
