#include "lili2d/scene/scene.hpp"

namespace lili {

Scene::Scene(const std::string &name) : name(name) {}

void Scene::handle_events(const Event& event) {
	(void)event;
}

void Scene::update(float dt) {
	(void)dt;
}

void Scene::fixed_update(float dt) {
	(void)dt;
}

void Scene::render(Renderer *renderer, float alpha) {
	(void)renderer;
	(void)alpha;
}

const std::string &Scene::get_name() const {
	return name;
}

}  // namespace lili
