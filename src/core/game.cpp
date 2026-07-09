#include "lili2d/core/game.hpp"
#include "lili2d/core/event.hpp"

namespace lili {

Game::Game(const std::string &title, int width, int height) {
	window = std::make_unique<Window>(title, width, height);
	renderer = std::make_unique<Renderer>(window.get());
}

Game::~Game() = default;

void Game::run() {
	onInit();
	running = true;

	while (running) {
		clock.update();

		Event event;
		while (event.poll()) {
			if (event.type() == EventType::QUIT) {
				running = false;
			}
			if (
				event.type() == EventType::KEYBOARD &&
				event.keyboard().key == SDLK_ESCAPE
			) {
				running = false;
			}
			onEvent(event);
		}

		onUpdate(clock.getDt());

		while (clock.step()) {
			onFixedUpdate(clock.getFixedDt());
		}

		if (renderer->beginFrame()) {
			onRender(clock.getAlpha());
			renderer->endFrame();
		}
	}

	onExit();
}

void Game::setTps(float value) {
	clock.setTps(value);
}

}  // namespace lili
