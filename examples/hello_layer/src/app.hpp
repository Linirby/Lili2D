#pragma once

#include <memory>
#include <lili/lili2d.hpp>

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

	lili::Sprite layer_1;
	lili::Sprite layer_2;
	lili::Sprite layer_3;

	lili::Rect red_square;
	int red_square_layer;

	lili::Text text_current_layer;
	lili::Text text_control_info;

	bool running;

	void handleEvents();
	void fixedUpdate(float dt);
	void render();
};
