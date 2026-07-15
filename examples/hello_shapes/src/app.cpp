#include "app.hpp"

App::App() : lili::Game("hello_shapes - Lili2D", 500, 350) {
	getWindow()->setResizable(true);
	lili::Renderer *renderer = getRenderer();

	thin_line = lili::Line(
		renderer,
		lili::LineShape({ 50.0f, 50.0f }, { 100.0f, 300.0f }, 1.0f),
		lili::Vec4(0.0f, 1.0f, 0.0f, 1.0f)
	);
	large_line = lili::Line(
		renderer,
		lili::LineShape({ 100.0f, 50.0f }, { 150.0f, 300.0f }, 5.0f),
		lili::Vec4(0.0f, 1.0f, 0.0f, 1.0f)
	);

	rect = lili::Rect(
		renderer,
		lili::RectShape(250.0f, 200.0f, 100.0f, 50.0f),
		lili::Vec4(1.0f, 0.0f, 0.0f, 1.0f)
	);

	clean_circle = lili::Circle(
		renderer,
		lili::CircleShape({ 225.0f, 100.0f }, 50.0f, 32.0f),
		lili::Vec4(0.0f, 0.0f, 1.0f, 1.0f)
	);
	low_poly_circle = lili::Circle(
		renderer,
		lili::CircleShape({ 375.0f, 100.0f }, 50.0f, 8.0f),
		lili::Vec4(0.0f, 0.0f, 1.0f, 1.0f)
	);

	rect.setHollowThickness(2.0f);
	clean_circle.setHollowThickness(2.0f);
	low_poly_circle.setHollowThickness(2.0f);
}

void App::onEvent(const lili::Event &event) {
	lili::KeyboardEvent kb = event.keyboard();

	if (event.type() == lili::EventType::KEYBOARD)
		if (kb.action == lili::KeyAction::PRESSED)
			if (kb.key == SDLK_ESCAPE)
				shutdown();

	if (event.type() == lili::EventType::KEYBOARD) {
		lili::KeyboardEvent keyboard = event.keyboard();
		if (keyboard.action == lili::KeyAction::PRESSED) {
			if (keyboard.key == SDLK_H || keyboard.key == SDLK_SPACE) {
				bool hollow = !rect.isHollow();
				rect.setHollow(hollow);
				clean_circle.setHollow(hollow);
				low_poly_circle.setHollow(hollow);
			}
		}
	}
}

void App::onRender(float alpha) {
	(void)alpha;
	thin_line.draw();
	large_line.draw();

	rect.draw();

	clean_circle.draw();
	low_poly_circle.draw();
}
