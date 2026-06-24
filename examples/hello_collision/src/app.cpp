#include "app.hpp"

App::App() {
	window = std::make_unique<lili::Window>(
		"hello_collision - Lili2D", 800, 800
	);
	renderer = std::make_unique<lili::Renderer>(window.get());

	cursor_rect = lili::Rect(
		renderer.get(), lili::RectShape(0, 0, 75, 75), lili::Vec4(0, 0, 1, 1)
	);
	cursor_circle = lili::Circle(
		renderer.get(),
		lili::CircleShape(lili::Vec2(0, 0), 30, 16),
		lili::Vec4(0, 0, 1, 1)
	);
	cursor_line = lili::Line(
		renderer.get(),
		lili::LineShape(lili::Vec2(0, 0), lili::Vec2(0, 0), 3),
		lili::Vec4(0, 0, 1, 1)
	);
	random_rect = lili::Rect(
		renderer.get(),
		lili::RectShape(350, 350, 100, 100),
		lili::Vec4(1, 0, 0, 1)
	);

	draw_rect = true;
	draw_circle = false;
	draw_line = false;

	running = true;
}

void App::run() {
	while (running) {
		handleEvents();
		update();
		render();
	}
}

void App::handleEvents() {
	lili::Event event;

	while (event.poll()) {
		lili::KeyboardEvent keyboard = event.keyboard();

		if (event.type() == lili::EventType::QUIT)
			running = false;
		if (keyboard.key == SDLK_ESCAPE)
			running = false;

		if (keyboard.action == lili::KeyAction::PRESSED) {
			if (keyboard.key == SDLK_1) {
				draw_rect = true;
				draw_circle = false;
				draw_line = false;
			}
			if (keyboard.key == SDLK_2) {
				draw_rect = false;
				draw_circle = true;
				draw_line = false;
			}
			if (keyboard.key == SDLK_3) {
				draw_rect = false;
				draw_circle = false;
				draw_line = true;
			}
		}
	}
}

void App::update() {
	lili::Mouse mouse;
	mouse.update();

	if (draw_rect) {
		cursor_rect.setPosition(mouse.getPos() - cursor_rect.getSize() * 0.5);
		if (lili::AABB2(cursor_rect).intersect(random_rect))
			cursor_rect.setColor(lili::Vec4(0, 1, 0, 1));
		else
			cursor_rect.setColor(lili::Vec4(0, 0, 1, 1));
	}
	if (draw_circle) {
		cursor_circle.setCenter(mouse.getPos());
		if (lili::AABB2(cursor_circle).intersect(random_rect))
			cursor_circle.setColor(lili::Vec4(0, 1, 0, 1));
		else
			cursor_circle.setColor(lili::Vec4(0, 0, 1, 1));
	}
	if (draw_line) {
		cursor_line.setEnd(
			mouse.getPos() - lili::Vec2(cursor_line.getThickness(), 0.0f) * 0.5
		);
		if (lili::AABB2(cursor_line).intersect(random_rect))
			cursor_line.setColor(lili::Vec4(0, 1, 0, 1));
		else
			cursor_line.setColor(lili::Vec4(0, 0, 1, 1));
	}

}

void App::render() {
	if (!renderer->beginFrame()) return;

	random_rect.draw();
	lili::Vec4 debug_color = lili::Vec4(0, 1, 0, 1);
	if (draw_rect) {
		cursor_rect.draw();
		lili::AABB2(cursor_rect).debugDraw(renderer.get(), debug_color);
	}
	if (draw_circle) {
		cursor_circle.draw();
		lili::AABB2(cursor_circle).debugDraw(renderer.get(), debug_color);
	}
	if (draw_line) {
		cursor_line.draw();
		lili::AABB2(cursor_line).debugDraw(renderer.get(), debug_color);
	}

	renderer->endFrame();
}
