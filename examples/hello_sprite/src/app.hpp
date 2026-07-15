#pragma once

#include <lili2d/lili2d.hpp>

class App : public lili::Game {
public:
	App();

private:
	lili::Sprite cat_sprite;

	void onEvent(const lili::Event &event) override;
	void onRender(float alpha) override;
};
