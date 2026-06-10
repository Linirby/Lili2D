#pragma once

#include <memory>
#include <lili/lili2d.hpp>

/// \brief App class.
class App {
public:
	/// \brief App method.
	App();

	/// \brief run method.
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

	/// \brief handleEvents method.
	void handleEvents();
	/// \brief update method.
	void update(float dt);
	/// \brief render method.
	void render();
};
