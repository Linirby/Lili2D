#pragma once

#include <lili2d/lili2d.hpp>
#include <memory>

class App : public lili::Game {
public:
    App();

private:
    lili::Keyboard keyboard;

    lili::Camera camera;
    lili::Circle camera_center;
    lili::Vec2 cam_pos;
    float camera_zoom;

    lili::Rect red_rect;
    lili::Rect green_rect;
    lili::Rect blue_rect;

    lili::Text text_cam_pos;
    lili::Text text_controls;

    void
    onEvent(const lili::Event& event) override;
    void
    onUpdate(float dt) override;
    void
    onRender(float alpha) override;
};
