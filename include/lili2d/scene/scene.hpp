#pragma once

#include <string>
#include "lili2d/render/renderer.hpp"
#include "lili2d/core/event.hpp"

namespace lili {

class SceneManager;

class Scene {
public:
	Scene(const std::string &name);
	virtual ~Scene() = default;

	virtual void on_enter() {}
	virtual void on_exit() {}
	virtual void on_pause() {}
	virtual void on_resume() {}

	virtual void handle_events(const Event& event);
	virtual void update(float dt);
	virtual void fixed_update(float dt);
	virtual void render(Renderer *renderer, float alpha);

	void set_manager(SceneManager *manager);

	const std::string &get_name() const;
	SceneManager *get_manager() const;

protected:
	std::string name;
	SceneManager *manager;
};

}  // namespace lili
