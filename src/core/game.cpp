#include "lili2d/core/game.hpp"
#include "lili2d/core/event.hpp"
#include "lili2d/core/thread_pool.hpp"

namespace lili {

Game::Game(
	const std::string &title, int width, int height, const EngineConfig& config
) {
	window = std::make_unique<Window>(title, width, height);

	SDL_GPUPresentMode preferred_present_mode = SDL_GPU_PRESENTMODE_MAILBOX;
	if (config.profile == PerformanceProfile::YES)
		preferred_present_mode = config.potato_present_mode;

	renderer = std::make_unique<Renderer>(window.get(), preferred_present_mode);
	this->config = config;
	thread_pool = std::make_unique<ThreadPool>(config);
}

ThreadPool* Game::getThreadPool() const {
	return thread_pool.get();
}

void Game::configure(const EngineConfig& new_config) {
	this->config = new_config;

	SDL_GPUPresentMode present_mode = SDL_GPU_PRESENTMODE_MAILBOX;
	if (config.profile == PerformanceProfile::YES)
		present_mode = config.potato_present_mode;
	if (renderer)
		renderer->setPresentMode(present_mode);

	thread_pool = std::make_unique<ThreadPool>(config);
}

const EngineConfig& Game::getConfig() const {
	return config;
}

Game::~Game() = default;

void Game::run() {
	onInit();
	running = true;

	while (running) {
		clock.update();

		Event event;
		while (event.poll()) {
			if (event.type() == EventType::QUIT)
				running = false;
			onEvent(event);
		}
		onUpdate(clock.getDt());
		while (clock.step())
			onFixedUpdate(clock.getFixedDt());
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
