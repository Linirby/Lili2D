#pragma once

#include <memory>
#include <string>

#include "lili2d/core/window.hpp"
#include "lili2d/core/clock.hpp"
#include "lili2d/render/renderer.hpp"
#include "lili2d/core/thread_pool.hpp"

namespace lili {

/// @brief Base class or container for system events.
class Event;

/// @brief Base class for managing the game lifecycle and main loop.
class Game {
public:
	/// @brief Constructs the Game application.
	/// @param title The window title.
	/// @param width The window width.
	/// @param height The window height.
	/// @param config The engine configuration settings.
	Game(
		const std::string &title,
		int width,
		int height,
		const EngineConfig& config = {}
	);

	/// @brief Destructor.
	virtual ~Game() = default;

	Game(Game &&other) noexcept = default;
	Game &operator=(Game &&other) noexcept = default;

	Game(const Game &) = delete;
	Game &operator=(const Game &) = delete;

	/// @brief Starts and runs the main game loop.
	void run();

	/// @brief Sets the fixed ticks per second.
	/// @param value Ticks per second rate.
	void setTps(float value);

	/// @brief Reconfigures the game's performance and threading settings at
	/// runtime.
	/// @param config The new engine configuration.
	void configure(const EngineConfig& config);

	/// @brief Gets the game's window.
	/// @return Pointer to the game's window.
	Window *getWindow() const;

	/// @brief Gets the renderer link to the game's window.
	/// @return Pointer to the used renderer.
	Renderer *getRenderer() const;

	/// @brief Gets the thread pool.
	/// @return Pointer to the thread pool.
	ThreadPool *getThreadPool() const;

	/// @brief Gets the active engine configuration.
	/// @return Reference to the active configuration.
	const EngineConfig &getConfig() const;

	/// @brief Gets the clock used in gameloop.
	/// @return Reference to the used clock.
	const Clock &getClock() const;

	/// @brief Called once when the game initializes.
	virtual void onInit();

	/// @brief Called when an SDL event is polled.
	/// @param event The polled event.
	virtual void onEvent(const Event &event);

	/// @brief Called once per frame for game logic updates.
	/// @param dt Delta time since the last frame.
	virtual void onUpdate(float dt);

	/// @brief Called at a fixed rate for physics updates.
	/// @param dt Fixed delta time.
	virtual void onFixedUpdate(float dt);

	/// @brief Called once per frame to render the game.
	/// @param alpha Interpolation factor between fixed updates.
	virtual void onRender(float alpha);

	/// @brief Called once when the game loop exits.
	virtual void onExit();

	/// @brief Stop the main gameloop
	void shutdown();

private:
	std::unique_ptr<Window> window;
	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<ThreadPool> thread_pool;
	EngineConfig config;
	Clock clock;
	bool running = false;
};

}  // namespace lili
