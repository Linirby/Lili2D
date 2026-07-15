#pragma once

#include <memory>
#include <lili2d/lili2d.hpp>

class App : public lili::Game {
public:
	App();

private:
	lili::Keyboard keyboard;

	lili::Sprite layer_1;
	lili::Sprite layer_2;
	lili::Sprite layer_3;

	lili::Rect red_square;
	int red_square_layer;

	std::unique_ptr<lili::BitmapFont> font;
	lili::Text text_current_layer;
	lili::Text text_control_info;

	void onEvent(const lili::Event &event) override;
	void onRender(float alpha) override;
};
