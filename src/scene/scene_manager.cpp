#include "lili2d/scene/scene_manager.hpp"

namespace lili {

void SceneManager::push(std::unique_ptr<Scene> scene) {
	scene->setManager(this);
	scenes.push_back(std::move(scene));
	scenes.back()->onEnter();
}

void SceneManager::pop() {
	if (scenes.empty()) return;
	scenes.back()->onExit();
	scenes.pop_back();
}

void SceneManager::changeScene(std::unique_ptr<Scene> scene) {
	scene->setManager(this);
	if (!scenes.empty()) {
		scenes.back()->onExit();
		scenes.pop_back();
	}
	scenes.push_back(std::move(scene));
	scenes.back()->onEnter();
}

void SceneManager::handleEvents(const Event &event) {
	if (!scenes.empty())
		scenes.back()->handleEvents(event);
}

void SceneManager::update(float dt) {
	if (!scenes.empty())
		scenes.back()->update(dt);
}

void SceneManager::fixedUpdate(float dt) {
	if (!scenes.empty())
		scenes.back()->fixedUpdate(dt);
}

void SceneManager::render(float alpha) {
	if (!scenes.empty())
		scenes.back()->render(alpha);
}

bool SceneManager::empty() const {
	return scenes.empty();
}

}  // namespace lili
