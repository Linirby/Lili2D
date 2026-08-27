#include "lili2d/scene/scene.hpp"

namespace lili {

Scene::Scene(const std::string& name, Renderer* renderer)
    : name(name), renderer(renderer) {}

void
Scene::handleEvents(const Event& event) {
    (void)event;
}

void
Scene::update(float dt) {
    (void)dt;
}

void
Scene::fixedUpdate(float dt) {
    (void)dt;
}

void
Scene::render(float alpha) {
    (void)alpha;
}

}  // namespace lili
