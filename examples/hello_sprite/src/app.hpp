#pragma once

#include <memory>
#include <lili/core.hpp>
#include <lili/render.hpp>

/// \brief App class.
class App {
public:
	/// \brief App method.
	App();
	~App() = default;

	/// \brief run method.
	void run();

private:
	std::unique_ptr<lili::Window> window;
	std::unique_ptr<lili::Renderer> renderer;
	lili::Sprite cat_sprite;

	bool running;

	/// \brief handle_events method.
	void handle_events();
	/// \brief render method.
	void render();
};
