#pragma once

#include <string>
#include "lili2d/render/renderer.hpp"
#include "lili2d/core/event.hpp"

namespace lili {

class SceneManager;

class Scene {
public:
	Scene(const std::string &name, Renderer *renderer);
	virtual ~Scene() = default;

	virtual void onEnter() {}
	virtual void onExit() {}
	virtual void onPause() {}
	virtual void onResume() {}

	virtual void handleEvents(const Event& event);
	virtual void update(float dt);
	virtual void fixedUpdate(float dt);
	virtual void render(float alpha);

	void setManager(SceneManager *manager);

	const std::string &getName() const;
	SceneManager *getManager() const;

protected:
	std::string name;
	Renderer *renderer;
	SceneManager *manager;
};

}  // namespace lili
