#include "lili2d/scene/scene.hpp"

namespace lili {

Scene::Scene(const std::string& name, Renderer* renderer)
    : name(name), renderer(renderer) {}

}  // namespace lili
