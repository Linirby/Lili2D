#pragma once

#include <lili2d/lili2d.hpp>

class MainScene;
class YetAgainScene;

/// @brief Secondary scene implementation for demonstration.
class AnotherScene : public lili::Scene {
public:
    /// @brief Constructor.
    /// @param renderer Renderer pointer.
    explicit AnotherScene(lili::Renderer* renderer);

    /// @brief Event handling callback.
    /// @param event Incoming event.
    void
    handleEvents(const lili::Event& event) override;
    /// @brief Frame render callback.
    /// @param alpha Interpolation factor.
    void
    render(float alpha) override;

private:
    /// @brief Title text.
    lili::Text text;
    /// @brief Info text.
    lili::Text info_text;
};
