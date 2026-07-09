#pragma once

#include <lili2d/lili2d.hpp>

class App : public lili::Game {
public:
	App();

private:
	lili::Line thin_line;
	lili::Line large_line;
	lili::Rect rect;
	lili::Circle clean_circle;
	lili::Circle low_poly_circle;

	void onEvent(const lili::Event &event) override;
	void onRender(float alpha) override;
};
