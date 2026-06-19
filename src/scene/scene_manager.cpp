#include "lili2d/scene/scene_manager.hpp"

namespace lili {

void SceneManager::push(std::unique_ptr<Scene> scene) {
	scenes.push_back(std::move(scene));
}

void SceneManager::pop() {
	if (scenes.empty()) return;
	scenes.pop_back();
}

void SceneManager::change_scene(std::unique_ptr<Scene> scene) {
	if (!scenes.empty())
		scenes.back() = std::move(scene);
	else
		scenes.push_back(std::move(scene));
}

void SceneManager::handle_events(const Event &event) {
	if (!scenes.empty())
		scenes.back()->handle_events(event);
}

void SceneManager::update(float dt) {
	if (!scenes.empty())
		scenes.back()->update(dt);
}

void SceneManager::fixed_update(float dt) {
	if (!scenes.empty())
		scenes.back()->fixed_update(dt);
}

void SceneManager::render(Renderer *renderer, float alpha) {
	if (!scenes.empty())
		scenes.back()->render(renderer, alpha);
}

}  // namespace lili
