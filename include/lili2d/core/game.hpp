#pragma once

#include <memory>
#include <string>

#include "lili2d/core/window.hpp"
#include "lili2d/core/clock.hpp"
#include "lili2d/render/renderer.hpp"

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
	Game(const std::string &title, int width, int height);

	/// @brief Destructor.
	virtual ~Game();

	Game(Game &&other) noexcept = default;
	Game &operator=(Game &&other) noexcept = default;

	Game(const Game &) = delete;
	Game &operator=(const Game &) = delete;

	/// @brief Starts and runs the main game loop.
	void run();

	/// @brief Sets the fixed ticks per second.
	/// @param value Ticks per second rate.
	void setTps(float value);

	/// @brief Called once when the game initializes.
	virtual void onInit() {}

	/// @brief Called once per frame for game logic updates.
	/// @param dt Delta time since the last frame.
	virtual void onUpdate(float dt) { (void)dt; }

	/// @brief Called at a fixed rate for physics updates.
	/// @param dt Fixed delta time.
	virtual void onFixedUpdate(float dt) { (void)dt; }

	/// @brief Called once per frame to render the game.
	/// @param alpha Interpolation factor between fixed updates.
	virtual void onRender(float alpha) { (void)alpha; }

	/// @brief Called once when the game loop exits.
	virtual void onExit() {}

	/// @brief Called when an SDL event is polled.
	/// @param event The polled event.
	virtual void onEvent(const Event &event) { (void)event; }

protected:
	std::unique_ptr<Window> window;
	std::unique_ptr<Renderer> renderer;
	bool running = false;

private:
	Clock clock;
};

}  // namespace lili
