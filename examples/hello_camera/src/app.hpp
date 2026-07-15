#pragma once

#include <memory>
#include <lili2d/lili2d.hpp>

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

	std::unique_ptr<lili::BitmapFont> font;
	lili::Text text_cam_pos;
	lili::Text text_controls;

	void onEvent(const lili::Event &event) override;
	void onUpdate(float dt) override;
	void onRender(float alpha) override;
};
