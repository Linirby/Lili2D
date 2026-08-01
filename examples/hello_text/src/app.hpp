#pragma once

#include <lili2d/lili2d.hpp>
#include <memory>

class App : public lili::Game {
public:
    App();

private:
    std::unique_ptr<lili::BitmapFont> font_example;
    float letter_spacing;
    float text_rotation = 0.0f;
    lili::Text welcome_text;
    lili::Text info_text;

    float speed_change;
    lili::Keyboard keyboard;

    void
    onEvent(const lili::Event& event) override;
    void
    onUpdate(float dt) override;
    void
    onRender(float alpha) override;
};
