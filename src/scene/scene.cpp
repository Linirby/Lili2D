#include "lili2d/scene/scene.hpp"

#include "lili2d/core/event.hpp"
#include "lili2d/render/renderer.hpp"

namespace lili {

Scene::Scene(const std::string& name, Renderer* renderer)
    : name(name), renderer(renderer) {}

}  // namespace lili
