#pragma once

#include <lili2d/lili2d.hpp>

/// @brief Application class for collision demonstration.
class App : public lili::Game {
public:
    /// @brief Default constructor initializing the App.
    App();

private:
    /// @brief Rect shape attached to cursor.
    lili::Rect cursor_rect;
    /// @brief Circle shape attached to cursor.
    lili::Circle cursor_circle;
    /// @brief Line shape attached to cursor.
    lili::Line cursor_line;
    /// @brief Highlight color tint.
    lili::Vec4 color;
    /// @brief Random target rectangle in scene.
    lili::Rect random_rect;

    /// @brief Toggle flag for drawing cursor rect.
    bool draw_rect = false;
    /// @brief Toggle flag for drawing cursor circle.
    bool draw_circle = false;
    /// @brief Toggle flag for drawing cursor line.
    bool draw_line = false;

    /// @brief Event handling callback.
    /// @param event Incoming event.
    void
    onEvent(const lili::Event& event) override;
    /// @brief Frame update callback.
    /// @param dt Delta time in seconds.
    void
    onUpdate(float dt) override;
    /// @brief Frame render callback.
    /// @param alpha Interpolation factor.
    void
    onRender(float alpha) override;
};
