#include "lili2d/core/game.hpp"

#include "lili2d/core/asset_manager.hpp"
#include "lili2d/core/event.hpp"
#include "lili2d/core/game_config.hpp"
#include "lili2d/core/thread_pool.hpp"
#include "lili2d/render/renderer.hpp"

namespace lili {

Game::Game(
    const std::string& title, int width, int height,
    const EngineConfig& engine_config
) {
    window = std::make_unique<Window>(title, width, height);

    GameConfig& config = GameConfig::get();
    config.setWindowSize(this, width, height);
    config.setWindowFullscreen(this, config.isWindowFullscreen());
    config.setWindowResizable(this, config.isWindowResizable());
    config.setWindowBorderless(this, config.isWindowBorderless());
    config.setRelativeMouseMode(this, config.isRelativeMouseMode());

    SDL_GPUPresentMode preferred_present_mode = SDL_GPU_PRESENTMODE_MAILBOX;
    if (engine_config.profile == PerformanceProfile::YES)
        preferred_present_mode = engine_config.potato_present_mode;

    renderer = std::make_unique<Renderer>(window.get(), preferred_present_mode);
    this->engine_config = engine_config;
    thread_pool = std::make_unique<ThreadPool>(engine_config);
}

Game::~Game() { AssetManager::clear(); }

void
Game::run() {
    onInit();
    running = true;

    while (running) {
        clock.update();

        Event event;
        while (event.poll()) {
            if (event.type() == EventType::QUIT) running = false;
            onEvent(event);
        }
        onUpdate(clock.getDt());
        AssetManager::checkHotReload();
        while (clock.step()) onFixedUpdate(clock.getFixedDt());
        if (renderer->beginFrame()) {
            onRender(clock.getAlpha());
            renderer->endFrame();
        }
    }
    onExit();
}

void
Game::configure(const EngineConfig& new_config) {
    this->engine_config = new_config;

    SDL_GPUPresentMode present_mode = SDL_GPU_PRESENTMODE_MAILBOX;
    if (engine_config.profile == PerformanceProfile::YES)
        present_mode = engine_config.potato_present_mode;
    if (renderer) renderer->setPresentMode(present_mode);

    thread_pool = std::make_unique<ThreadPool>(engine_config);
}

void
Game::onInit() {}

void
Game::onEvent(const Event& event) {
    if (event.type() == lili::EventType::WINDOW) {
        lili::GameConfig& conf = lili::GameConfig::get();
        lili::WindowEvent win_event = event.window();
        switch (win_event.type) {
            case lili::WindowEventType::WINDOW_RESIZED:
            case lili::WindowEventType::WINDOW_PIXEL_SIZE_CHANGED:
                conf.updateWindowSize(win_event.data1, win_event.data2);
                break;
            case lili::WindowEventType::WINDOW_ENTER_FULLSCREEN:
                conf.setWindowFullscreen(this, true);
                break;
            case lili::WindowEventType::WINDOW_LEAVE_FULLSCREEN:
                conf.setWindowFullscreen(this, false);
                break;
            default:
                break;
        }
    } else if (event.type() == lili::EventType::KEYBOARD) {
        lili::KeyboardEvent kb = event.keyboard();
        if (kb.action == lili::KeyAction::PRESSED && kb.key == SDLK_F11) {
            lili::GameConfig& conf = lili::GameConfig::get();
            conf.setWindowFullscreen(this, !conf.isWindowFullscreen());
        }
    }
}

void
Game::onUpdate(float dt) {
    (void)dt;
}

void
Game::onFixedUpdate(float dt) {
    (void)dt;
}

void
Game::onRender(float alpha) {
    (void)alpha;
}

void
Game::onExit() {}

}  // namespace lili
