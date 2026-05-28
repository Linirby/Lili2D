#include <stdexcept>
#include <SDL3/SDL_init.h>
#include "core/window.hpp"

namespace lili {

static int window_count = 0;

Window::Window(const std::string &title, int width, int height) {
	if (window_count == 0) {
		if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
			throw std::runtime_error("Failed to initialize SDL: " + std::string(SDL_GetError()));
		}
	}
	window_count++;

	resizable = false;
	borderless = false;
	fullscreen = false;
	window = SDL_CreateWindow(title.c_str(), width, height, 0);
	if (!window) {
		throw std::runtime_error("Failed to create SDL_Window: " + std::string(SDL_GetError()));
	}
}

Window::~Window() {
	if (window) SDL_DestroyWindow(window);

	window_count--;
	if (window_count == 0) {
		SDL_Quit();
	}
}

void Window::set_title(const std::string &title) {
	if (!SDL_SetWindowTitle(window, title.c_str()))
		throw std::runtime_error(
			"Failed to change window name: " + std::string(SDL_GetError())
		);
}

void Window::set_size(int width, int height) {
	if (!SDL_SetWindowSize(window, width, height))
		throw std::runtime_error(
			"Failed to change window size: " + std::string(SDL_GetError())
		);
}

void Window::set_resizable(bool activate) {
	if (!SDL_SetWindowResizable(window, activate))
		throw std::runtime_error(
			"Failed to change window to resizable: " +
			std::string(SDL_GetError())
		);
	resizable = activate;
}

void Window::set_borderless(bool activate) {
	if (!SDL_SetWindowBordered(window, !activate))
		throw std::runtime_error(
			"Failed to change window to borderless: " +
			std::string(SDL_GetError())
		);
	borderless = activate;
}

void Window::set_fullscreen(bool activate) {
	if (!SDL_SetWindowFullscreen(window, activate))
		throw std::runtime_error(
			"Failed to change window to fullscreen mode: " +
			std::string(SDL_GetError())
		);
	fullscreen = activate;
}

void Window::set_relative_mouse_mode(bool activate) {
	if (!SDL_SetWindowRelativeMouseMode(window, activate))
		throw std::runtime_error(
			"Failed to change window to relative mouse mode: " +
			std::string(SDL_GetError())
		);
}

const std::string Window::get_title() const {
	return std::string(SDL_GetWindowTitle(window));
}

int Window::get_width() const {
	int w = 0;
	if (!SDL_GetWindowSize(window, &w, nullptr))
		throw std::runtime_error(
			"Failed to get window size: " + std::string(SDL_GetError())
		);
	return w;
}

int Window::get_height() const {
	int h = 0;
	if (!SDL_GetWindowSize(window, nullptr, &h))
		throw std::runtime_error(
			"Failed to get window size: " + std::string(SDL_GetError())
		);
	return h;
}

std::array<int, 2> Window::get_size() const {
	int w, h = 0;
	if (!SDL_GetWindowSize(window, &w, &h))
		throw std::runtime_error(
			"Failed to get window size: " + std::string(SDL_GetError())
		);
	return {w, h};
}

bool Window::is_resizable() const {
	return resizable;
}

bool Window::is_borderless() const {
	return borderless;
}

bool Window::is_fullscreen() const {
	return fullscreen;
}

bool Window::is_relative_mouse_mode() const {
	return SDL_GetWindowRelativeMouseMode(window);
}

SDL_Window *Window::get_sdl_window() const {
	return window;
}

}  // namespace lili
