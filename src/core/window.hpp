#pragma once

#include <string>
#include <array>
#include <SDL3/SDL.h>

namespace lili {

class Window {
public:
	Window();
	~Window();

	void set_title(const std::string &title);
	void set_size(int width, int height);
	void set_resizable(bool activate);
	void set_borderless(bool activate);
	void set_fullscreen(bool activate);
	void set_relative_mouse_mode(bool activate);

	const std::string get_title() const;
	int get_width() const;
	int get_height() const;
	std::array<int, 2> get_size() const;
	bool is_resizable() const;
	bool is_borderless() const;
	bool is_fullscreen() const;
	bool is_relative_mouse_mode() const;

	SDL_Window *get_sdl_window() const;

private:
	SDL_Window *window;
	bool resizable, borderless, fullscreen;
};

}  // namespace lili
