#pragma once

#include <memory>
#include <lili/lili2d.hpp>

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
	lili::Line thin_line;
	lili::Line large_line;
	lili::Rect rect;
	lili::Circle clean_circle;
	lili::Circle low_poly_circle;

	bool running;

	/// \brief handle_events method.
	void handle_events();
	/// \brief render method.
	void render();
};
