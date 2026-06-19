#pragma once

#include <vector>
#include <memory>
#include "lili2d/scene/scene.hpp"

namespace lili {

class SceneManager {
public:
	void push(std::unique_ptr<Scene> scene);
	void pop();

	void change_scene(std::unique_ptr<Scene> scene);

	void handle_events(const Event &event);
	void update(float dt);
	void fixed_update(float dt);
	void render(Renderer *renderer, float alpha);

private:
	std::vector<std::unique_ptr<Scene>> scenes;
};

}  // namespace lili
