#pragma once

#include <lili2d/lili2d.hpp>

class App : public lili::Game {
public:
	App();

private:
	lili::Rect cursor_rect;
	lili::Circle cursor_circle;
	lili::Line cursor_line;
	lili::Vec4 color;
	lili::Rect random_rect;

	bool draw_rect;
	bool draw_circle;
	bool draw_line;

	void onEvent(const lili::Event &event) override;
	void onUpdate(float dt) override;
	void onRender(float alpha) override;
};
