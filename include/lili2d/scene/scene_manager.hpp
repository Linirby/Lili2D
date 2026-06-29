#pragma once

#include <vector>
#include <memory>
#include "lili2d/scene/scene.hpp"

namespace lili {

class SceneManager {
public:
	void push(std::unique_ptr<Scene> scene);
	void pop();

	void changeScene(std::unique_ptr<Scene> scene);

	void handleEvents(const Event &event);
	void update(float dt);
	void fixedUpdate(float dt);
	void render(float alpha);

	bool empty() const;

private:
	std::vector<std::unique_ptr<Scene>> scenes;
};

}  // namespace lili
