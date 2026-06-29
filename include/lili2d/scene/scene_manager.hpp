#pragma once

#include <vector>
#include <memory>
#include "lili2d/scene/scene.hpp"

namespace lili {

/// @brief Manager for transitioning and updating scenes.
class SceneManager {
public:
	/// @brief Pushes a new scene onto the stack.
	/// @param scene The scene to push.
	void push(std::unique_ptr<Scene> scene);
	/// @brief Pops the top scene from the stack.
	void pop();

	/// @brief Replaces the top scene with a new one.
	/// @param scene The new scene.
	void changeScene(std::unique_ptr<Scene> scene);

	/// @brief Dispatches an event to the top scene.
	/// @param event The event.
	void handleEvents(const Event &event);
	/// @brief Updates the top scene.
	/// @param dt Delta time in seconds.
	void update(float dt);
	/// @brief Updates the top scene's physics.
	/// @param dt Delta time in seconds.
	void fixedUpdate(float dt);
	/// @brief Renders the top scene.
	/// @param alpha Interpolation factor.
	void render(float alpha);

	/// @brief Checks if the scene stack is empty.
	/// @return True if empty, false otherwise.
	bool empty() const;

private:
	std::vector<std::unique_ptr<Scene>> scenes;
};

}  // namespace lili
