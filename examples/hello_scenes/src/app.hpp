#pragma once

#include <lili2d/lili2d.hpp>

/// @brief Application class for scenes demonstration.
class App : public lili::Game {
public:
    /// @brief Default constructor initializing the App.
    App();

private:
    /// @brief Scene manager stack.
    lili::SceneManager scene_manager;

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
