#pragma once

#include <memory>
#include <lili/lili2d.hpp>

/// \brief App class.
class App {
public:
	/// \brief App method.
	App();
	~App() = default;

	/// \brief run method.
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

	/// \brief handle_events method.
	void handle_events();
	/// \brief fixed_update method.
	void fixed_update(float dt);
	/// \brief render method.
	void render();
};
