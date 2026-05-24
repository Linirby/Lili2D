#pragma once

#include "render/scene/camera.hpp"  // IWYU pragma: keep
#include "render/scene/material.hpp"  // IWYU pragma: keep
#include "render/scene/model.hpp"  // IWYU pragma: keep
#include "render/scene/sprite.hpp"  // IWYU pragma: keep
#include "render/scene/text.hpp"  // IWYU pragma: keep

#include "render/renderer.hpp"  // IWYU pragma: keep

namespace lili {

class Camera;

struct MaterialProps;
struct Material;
class MaterialRegistry;

struct Model;

class Sprite;

class BitmapFont;
class Text;

class Renderer;

}  // namespace lili
