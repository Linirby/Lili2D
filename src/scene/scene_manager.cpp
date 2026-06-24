#include "lili2d/scene/scene_manager.hpp"

namespace lili {

void SceneManager::push(std::unique_ptr<Scene> scene) {
	scene->set_manager(this);
	scenes.push_back(std::move(scene));
	scenes.back()->on_enter();
}

void SceneManager::pop() {
	if (scenes.empty()) return;
	scenes.back()->on_exit();
	scenes.pop_back();
}

void SceneManager::change_scene(std::unique_ptr<Scene> scene) {
	scene->set_manager(this);
	if (!scenes.empty()) {
		scenes.back()->on_exit();
		scenes.pop_back();
	}
	scenes.push_back(std::move(scene));
	scenes.back()->on_enter();
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

bool SceneManager::empty() const {
	return scenes.empty();
}

}  // namespace lili
