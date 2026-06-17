#pragma once

#include <memory>
#include <lili2d/lili2d.hpp>

class App {
public:
	App();

	void run();

private:
	std::unique_ptr<lili::Window> window;
	std::unique_ptr<lili::Renderer> renderer;
	std::unique_ptr<lili::BitmapFont> font;

	lili::Clock clock;
	lili::Keyboard keyboard;

	lili::Camera camera;

	lili::Rect red_rect;
	lili::Rect green_rect;
	lili::Rect blue_rect;

	lili::Circle camera_center;
	lili::Vec2 cam_pos;
	float camera_zoom;

	lili::Text text_cam_pos;
	lili::Text text_controls;

	bool running;

	void handleEvents();
	void update(float dt);
	void render();
};
