#pragma once

#include <memory>
#include <lili/lili2d.hpp>

class App {
public:
	App();
	~App() = default;

	void run();

private:
	std::unique_ptr<lili::Window> window;
	std::unique_ptr<lili::Renderer> renderer;
	std::unique_ptr<lili::BitmapFont> font;

	lili::Clock clock;
	lili::Keyboard keyboard;

	lili::Camera camera;
	lili::Text text_cam_pos;

	lili::Rect red_rect;
	lili::Rect green_rect;
	lili::Rect blue_rect;

	lili::Circle camera_center;
	lili::Vec2 cam_pos;
	float camera_zoom;

	bool running;

	void handle_events();
	void update(float dt);
	void render();
};
