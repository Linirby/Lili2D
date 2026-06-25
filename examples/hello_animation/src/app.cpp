#include "app.hpp"

App::App() {
	window = std::make_unique<lili::Window>(
		"hello_animation - Lili2D", 400, 400
	);
	window->setResizable(true);
	renderer = std::make_unique<lili::Renderer>(window.get());
	clock = lili::Clock(20.0f);

	head_atlas = lili::AtlasMap(renderer.get(), "head_animation.png");
	head_atlas.slice(8, 1);

	head_sprite = lili::AnimatedSprite(
		renderer.get(), lili::Animation(head_atlas.getSliceUVs(0, 8))
	);
	head_sprite.setScale({ 15, 15 });
	head_sprite.setFrameSpeed(0.2f);

	running = true;
}

void App::run() {
	while (running) {
		clock.update();
		handleEvents();
		update(clock.getDt());
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

void App::update(float dt) {
	head_sprite.setPosition(lili::Vec2(
		(float)window->getWidth() / 2 - head_sprite.getWidth() / 2,
		(float)window->getHeight() - head_sprite.getHeight()
	));
	head_sprite.update(dt);
}

void App::render() {
	if (!renderer->beginFrame()) return;

	head_sprite.draw();

	renderer->endFrame();
}
