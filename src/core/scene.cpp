#include "lili2d/core/scene.hpp"

#include "lili2d/render/renderer.hpp"

namespace lili {

Scene::Scene(const std::string& name, Renderer* renderer)
  : name(name)
  , renderer(renderer)
{
}

} // namespace lili
