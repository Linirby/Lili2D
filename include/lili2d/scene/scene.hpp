#pragma once

#include <string>
#include "lili2d/render/renderer.hpp"
#include "lili2d/core/event.hpp"

namespace lili {

/// @brief Manager for transitioning and updating scenes.
class SceneManager;

/// @brief Represents a single game state or screen.
class Scene {
public:
	Scene(const std::string &name, Renderer *renderer);
	virtual ~Scene() = default;
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	Scene(Scene&&) = delete;
	Scene& operator=(Scene&&) = delete;

	/// @brief Called when the scene becomes active.
	virtual void onEnter() {}
	/// @brief Called when the scene becomes inactive.
	virtual void onExit() {}
	/// @brief Called when another scene is pushed on top of this one.
	virtual void onPause() {}
	/// @brief Called when a scene on top of this one is popped.
	virtual void onResume() {}

	/// @brief Handles an incoming event.
	/// @param event The event.
	virtual void handleEvents(const Event& event);
	/// @brief Updates the scene logic.
	/// @param dt Delta time in seconds.
	virtual void update(float dt);
	/// @brief Updates the scene physics logic.
	/// @param dt Delta time in seconds.
	virtual void fixedUpdate(float dt);
	/// @brief Renders the scene.
	/// @param alpha Interpolation factor.
	virtual void render(float alpha);

	/// @brief Sets the parent scene manager.
	/// @param manager The manager.
	void setManager(SceneManager *manager);

	/// @brief Gets the scene name.
	/// @return The name.
	const std::string &getName() const;
	/// @brief Gets the parent scene manager.
	/// @return The manager.
	SceneManager *getManager() const;

protected:
	std::string name;
	Renderer *renderer;
	SceneManager *manager;
};

}  // namespace lili
