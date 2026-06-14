#include <stdexcept>
#include <SDL3/SDL_init.h>
#include "window.hpp"

namespace lili {

static int window_count = 0;

Window::Window(const std::string &title, int width, int height) {
	if (window_count == 0) {
		if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
			throw std::runtime_error(
				"Failed to initialize SDL: " + std::string(SDL_GetError())
			);
		}
	}
	window_count++;
	window = SDL_CreateWindow(title.c_str(), width, height, 0);
	if (!window) {
		throw std::runtime_error(
			"Failed to create SDL_Window: " + std::string(SDL_GetError())
		);
	}
}

Window::~Window() {
	if (window) {
		SDL_DestroyWindow(window);
		window_count--;
		if (window_count == 0) {
			SDL_Quit();
		}
	}
}

Window::Window(Window &&other) noexcept
	: resizable(other.resizable),
			borderless(other.borderless),
			fullscreen(other.fullscreen),
			window(other.window) {
	other.window = nullptr;
}

Window& Window::operator=(Window &&other) noexcept {
	if (this != &other) {
		if (window) {
			SDL_DestroyWindow(window);
			window_count--;
			if (window_count == 0) {
				SDL_Quit();
			}
		}
		resizable = other.resizable;
		borderless = other.borderless;
		fullscreen = other.fullscreen;
		window = other.window;
		other.window = nullptr;
	}
	return *this;
}

void Window::setTitle(const std::string &title) {
	if (!SDL_SetWindowTitle(window, title.c_str()))
		throw std::runtime_error(
			"Failed to change window name: " + std::string(SDL_GetError())
		);
}

void Window::setSize(int width, int height) {
	if (!SDL_SetWindowSize(window, width, height))
		throw std::runtime_error(
			"Failed to change window size: " + std::string(SDL_GetError())
		);
}

void Window::setResizable(bool activate) {
	if (!SDL_SetWindowResizable(window, activate))
		throw std::runtime_error(
			"Failed to change window to resizable: " +
			std::string(SDL_GetError())
		);
	resizable = activate;
}

void Window::setBorderless(bool activate) {
	if (!SDL_SetWindowBordered(window, !activate))
		throw std::runtime_error(
			"Failed to change window to borderless: " +
			std::string(SDL_GetError())
		);
	borderless = activate;
}

void Window::setFullscreen(bool activate) {
	if (!SDL_SetWindowFullscreen(window, activate))
		throw std::runtime_error(
			"Failed to change window to fullscreen mode: " +
			std::string(SDL_GetError())
		);
	fullscreen = activate;
}

void Window::setRelativeMouseMode(bool activate) {
	if (!SDL_SetWindowRelativeMouseMode(window, activate))
		throw std::runtime_error(
			"Failed to change window to relative mouse mode: " +
			std::string(SDL_GetError())
		);
}

const std::string Window::get_title() const {
	return std::string(SDL_GetWindowTitle(window));
}

int Window::getWidth() const {
	int w = 0;
	if (!SDL_GetWindowSize(window, &w, nullptr))
		throw std::runtime_error(
			"Failed to get window size: " + std::string(SDL_GetError())
		);
	return w;
}

int Window::getHeight() const {
	int h = 0;
	if (!SDL_GetWindowSize(window, nullptr, &h))
		throw std::runtime_error(
			"Failed to get window size: " + std::string(SDL_GetError())
		);
	return h;
}

Vec2 Window::getSize() const {
	int w, h = 0;
	if (!SDL_GetWindowSize(window, &w, &h))
		throw std::runtime_error(
			"Failed to get window size: " + std::string(SDL_GetError())
		);
	return {(float)w, (float)h};
}

bool Window::isResizable() const {
	return resizable;
}

bool Window::isBorderless() const {
	return borderless;
}

bool Window::isFullscreen() const {
	return fullscreen;
}

bool Window::isRelativeMouseMode() const {
	return SDL_GetWindowRelativeMouseMode(window);
}

SDL_Window *Window::getSdlWindow() const {
	return window;
}

}  // namespace lili
