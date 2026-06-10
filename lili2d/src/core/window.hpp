#pragma once

#include <string>
#include <SDL3/SDL.h>

#include "geometry/vec2.hpp"

namespace lili {

/// @brief Represents an application window.
class Window {
public:
	/// @brief Constructs a window with the specified title and dimensions.
	/// @param title The window title.
	/// @param width The width of the window in pixels.
	/// @param height The height of the window in pixels.
	Window(const std::string &title, int width, int height);

	/// @brief Destroys the window and cleans up resources.
	~Window();

	/// @brief Move constructor.
	Window(Window &&other) noexcept;
	/// @brief Move assignment.
	Window& operator=(Window &&other) noexcept;

	/// @brief Copy constructor is deleted to prevent double-freeing the
	/// SDL_Window pointer.
	Window(const Window &) = delete;
	/// @brief Copy assignment is deleted to prevent double-freeing the
	/// SDL_Window pointer.
	Window& operator=(const Window &) = delete;

	/// @brief Sets the window title.
	/// @param title The new title.
	void setTitle(const std::string &title);
	/// @brief Sets the window size.
	/// @param width The new width in pixels.
	/// @param height The new height in pixels.
	void setSize(int width, int height);
	/// @brief Enables or disables window resizing.
	/// @param activate True to enable resizing, false to disable.
	void setResizable(bool activate);
	/// @brief Enables or disables the window border.
	/// @param activate True to make borderless, false to show border.
	void setBorderless(bool activate);
	/// @brief Enables or disables fullscreen mode.
	/// @param activate True to enable fullscreen, false to disable.
	void setFullscreen(bool activate);
	/// @brief Enables or disables relative mouse mode.
	/// @param activate True to enable relative mouse mode, false to disable.
	void setRelativeMouseMode(bool activate);

	/// @brief Gets the current window title.
	/// @return The window title.
	const std::string get_title() const;
	/// @brief Gets the window width.
	/// @return The width in pixels.
	int getWidth() const;
	/// @brief Gets the window height.
	/// @return The height in pixels.
	int getHeight() const;
	/// @brief Gets the window size.
	/// @return An array containing width and height.
	Vec2 getSize() const;

	/// @brief Checks if the window is resizable.
	/// @return True if resizable, false otherwise.
	bool isResizable() const;
	/// @brief Checks if the window is borderless.
	/// @return True if borderless, false otherwise.
	bool isBorderless() const;
	/// @brief Checks if the window is in fullscreen mode.
	/// @return True if fullscreen, false otherwise.
	bool isFullscreen() const;
	/// @brief Checks if relative mouse mode is enabled.
	/// @return True if relative mouse mode is enabled, false otherwise.
	bool isRelativeMouseMode() const;

	/// @brief Gets the underlying SDL_Window pointer.
	/// @return Pointer to the SDL_Window.
	SDL_Window *getSdlWindow() const;

private:
	bool resizable = false;
	bool borderless = false;
	bool fullscreen = false;
	SDL_Window *window = nullptr;
};

}  // namespace lili
