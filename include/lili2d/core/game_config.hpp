#pragma once

namespace lili {

class Game;

class GameConfig {
public:
    int
    getWindowWidth() const;
    int
    getWindowHeight() const;
    bool
    isWindowFullscreen() const;
    bool
    isWindowResizable() const;
    bool
    isWindowBorderless() const;
    bool
    isRelativeMouseMode() const;

    void
    setWindowSize(lili::Game* game, int width, int height);
    void
    updateWindowSize(int width, int height);

    void
    setWindowFullscreen(lili::Game* game, bool set_fullscreen);
    void
    updateWindowFullscreen(bool set_fullscreen);

    void
    setWindowResizable(lili::Game* game, bool resizable);
    void
    updateWindowResizable(bool resizable);

    void
    setWindowBorderless(lili::Game* game, bool borderless);
    void
    updateWindowBorderless(bool borderless);

    void
    setRelativeMouseMode(lili::Game* game, bool relative_mouse);
    void
    updateRelativeMouseMode(bool relative_mouse);

    static GameConfig&
    get();

private:
    int window_w = 800;
    int window_h = 600;
    bool window_fullscreen = false;
    bool window_resizable = false;
    bool window_borderless = false;
    bool relative_mouse_mode = false;
};

}  // namespace lili
