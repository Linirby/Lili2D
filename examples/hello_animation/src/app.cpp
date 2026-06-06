#include "app.hpp"

App::App() {
	window = std::make_unique<lili::Window>(
		"hello_animation - Lili2D", 400, 400
	);
	window->set_resizable(true);
	renderer = std::make_unique<lili::Renderer>(window.get());
	clock = lili::Clock(20.0f);

	head_animation = lili::Animation(renderer.get(), "head_animation.png");
	head_animation.slice(8, 1);

	head_sprite = lili::AnimatedSprite(renderer.get(), head_animation);
	head_sprite.set_scale({ 15, 15 });
	head_sprite.set_frame_speed(0.2f);

	running = true;
}

void App::run() {
	while (running) {
		clock.update();
		handle_events();
		update(clock.get_dt());
		render();
	}
}

void App::handle_events() {
	lili::Event event;

	while (event.poll()) {
		lili::KeyboardEvent keyboard = event.keyboard();

		if (event.type() == lili::EventType::QUIT)
			running = false;
		if (keyboard.key == SDLK_ESCAPE)
			running = false;
	}
}

void App::update(float dt) {
	head_sprite.set_position(lili::Vec2(
		(float)window->get_width() / 2 - head_sprite.get_width() / 2,
		(float)window->get_height() - head_sprite.get_height()
	));
	head_sprite.update(dt);
}

void App::render() {
	if (!renderer->begin_frame()) return;

	head_sprite.draw();

	renderer->end_frame();
}
