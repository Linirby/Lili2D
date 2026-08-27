#pragma once

namespace lili {

class Game;

/// @brief Global runtime configuration and window state manager.
class GameConfig {
public:
    /// @brief Gets the configured window width.
    /// @return Width in pixels.
    [[nodiscard]] inline int
    getWindowWidth() const noexcept {
        return window_w;
    }

    /// @brief Gets the configured window height.
    /// @return Height in pixels.
    [[nodiscard]] inline int
    getWindowHeight() const noexcept {
        return window_h;
    }

    /// @brief Checks if fullscreen mode is enabled.
    /// @return True if fullscreen is active.
    [[nodiscard]] inline bool
    isWindowFullscreen() const noexcept {
        return window_fullscreen;
    }

    /// @brief Checks if window resizability is enabled.
    /// @return True if resizable.
    [[nodiscard]] inline bool
    isWindowResizable() const noexcept {
        return window_resizable;
    }

    /// @brief Checks if borderless mode is enabled.
    /// @return True if borderless.
    [[nodiscard]] inline bool
    isWindowBorderless() const noexcept {
        return window_borderless;
    }

    /// @brief Checks if relative mouse mode is enabled.
    /// @return True if relative mouse mode is active.
    [[nodiscard]] inline bool
    isRelativeMouseMode() const noexcept {
        return relative_mouse_mode;
    }

    /// @brief Sets the window size and updates the target Game window.
    /// @param game Pointer to Game instance.
    /// @param width New width in pixels.
    /// @param height New height in pixels.
    void
    setWindowSize(lili::Game* game, int width, int height);

    /// @brief Updates internal window size state without applying to window.
    /// @param width Width in pixels.
    /// @param height Height in pixels.
    inline void
    updateWindowSize(int width, int height) noexcept {
        if (width > 0 && height > 0) {
            window_w = width;
            window_h = height;
        }
    }

    /// @brief Sets window fullscreen mode and updates the target Game window.
    /// @param game Pointer to Game instance.
    /// @param set_fullscreen True to enable fullscreen.
    void
    setWindowFullscreen(lili::Game* game, bool set_fullscreen);

    /// @brief Updates internal fullscreen state without applying to window.
    /// @param set_fullscreen True for fullscreen.
    inline void
    updateWindowFullscreen(bool set_fullscreen) noexcept {
        window_fullscreen = set_fullscreen;
    }

    /// @brief Sets window resizable state and updates the target Game window.
    /// @param game Pointer to Game instance.
    /// @param resizable True to allow resizing.
    void
    setWindowResizable(lili::Game* game, bool resizable);

    /// @brief Updates internal resizable state without applying to window.
    /// @param resizable True for resizable.
    inline void
    updateWindowResizable(bool resizable) noexcept {
        window_resizable = resizable;
    }

    /// @brief Sets window borderless state and updates the target Game window.
    /// @param game Pointer to Game instance.
    /// @param borderless True for borderless mode.
    void
    setWindowBorderless(lili::Game* game, bool borderless);

    /// @brief Updates internal borderless state without applying to window.
    /// @param borderless True for borderless.
    inline void
    updateWindowBorderless(bool borderless) noexcept {
        window_borderless = borderless;
    }

    /// @brief Sets relative mouse mode and updates the target Game window.
    /// @param game Pointer to Game instance.
    /// @param relative_mouse True for relative mouse mode.
    void
    setRelativeMouseMode(lili::Game* game, bool relative_mouse);

    /// @brief Updates internal relative mouse mode state without applying to
    /// window.
    /// @param relative_mouse True for relative mouse mode.
    inline void
    updateRelativeMouseMode(bool relative_mouse) noexcept {
        relative_mouse_mode = relative_mouse;
    }

    /// @brief Gets the singleton instance of GameConfig.
    /// @return Reference to GameConfig.
    [[nodiscard]] static GameConfig&
    get();

private:
    /// @brief Configured window width in pixels.
    int window_w = 800;
    /// @brief Configured window height in pixels.
    int window_h = 600;
    /// @brief Flag indicating if fullscreen is enabled.
    bool window_fullscreen = false;
    /// @brief Flag indicating if window is resizable.
    bool window_resizable = false;
    /// @brief Flag indicating if window is borderless.
    bool window_borderless = false;
    /// @brief Flag indicating if relative mouse mode is enabled.
    bool relative_mouse_mode = false;
};

}  // namespace lili
