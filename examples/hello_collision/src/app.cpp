#include "app.hpp"

App::App() {
	window = std::make_unique<lili::Window>(
		"hello_collision - Lili2D", 800, 800
	);
	renderer = std::make_unique<lili::Renderer>(window.get());

	cursor_rect = lili::Rect(
		renderer.get(), lili::RectShape(0, 0, 75, 75), lili::Vec4(0, 0, 1, 1)
	);
	random_rect = lili::Rect(
		renderer.get(),
		lili::RectShape(350, 350, 100, 100),
		lili::Vec4(1, 0, 0, 1)
	);

	running = true;
}

void App::run() {
	while (running) {
		handleEvents();
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
	}
}

void App::render() {
	lili::Mouse mouse;
	mouse.update();

	if (!renderer->beginFrame()) return;

	cursor_rect.setPosition(mouse.getPos() - cursor_rect.getSize() * 0.5);
	lili::AABB cursor_bounding = lili::AABB(
		cursor_rect.getPosition(), cursor_rect.getSize()
	);
	bool collide = (
		cursor_bounding.intersect(
			lili::AABB(random_rect.getPosition(), random_rect.getSize())
		)
	);
	if (collide)
		cursor_rect.setColor(lili::Vec4(0, 1, 0, 1));
	else
		cursor_rect.setColor(lili::Vec4(0, 0, 1, 1));
	random_rect.draw();
	cursor_rect.draw();

	renderer->endFrame();
}
