# Changelog & ECS Migration Guide

All notable changes to **Lili2D** and migration instructions from the legacy OOP rendering API to the modern Data-Oriented ECS architecture are documented in this file.

---

## [Unreleased] - ECS Render Modernization

### Overview

Lili2D has transitioned from an inheritance-based OOP rendering model (`IRenderable` subclasses with manual `.draw()` calls) to a fully data-oriented **Entity Component System (ECS)** pipeline powered by `Render2DSystem`, `UILayoutSystem`, `AnimationSystem`, `DynamicBatch`, `StaticBatch`, `StringArena`, and `MaterialRegistry`.

- **Automatic Rendering**: Entities with 2D components (`cSprite`, `cRect`, `cCircle`, `cLine`, `cText`) are automatically collected, state-sorted, and batched every frame by `Render2DSystem` inside `Game` / `Scene`. Manual `.draw()` calls in `onRender()` are no longer needed.
- **Trivially Copyable Components**: All 2D components are lightweight POD structs stored in contiguous `ComponentPool<T>` arrays.
- **Component Naming Convention (`XComponent` -> `cX`)**: All ECS components have been renamed from the `XComponent` suffix convention to a `cX` prefix convention, where `X` is the component name (e.g. `TransformComponent` -> `cTransform`, `Render2DComponent` -> `cRender2D`, `SpriteComponent` -> `cSprite`, `AnimationComponent` -> `cAnimation`, `UILayoutComponent` -> `cUILayout`, etc.).

---

### Replacement Reference Table

| Removed OOP Class / Method                            | New ECS Component(s)                                  | New Helper / System                                       |
| :---------------------------------------------------- | :---------------------------------------------------- | :-------------------------------------------------------- |
| `lili::IRenderable`                                   | `cTransform` + `cRender2D`                            | Processed by `Render2DSystem`                             |
| `lili::Sprite`                                        | `cSprite` + `cRender2D` + `cTransform`                | `lili::createSprite(...)`                                 |
| `lili::AnimatedSprite`                                | `cAnimation` + `cSprite` + `cRender2D` + `cTransform` | `lili::createAnimatedSprite(...)` + `AnimationSystem`     |
| `lili::Rect`                                          | `cRect` + `cRender2D` + `cTransform`                  | `lili::createRect(...)`                                   |
| `lili::Circle`                                        | `cCircle` + `cRender2D` + `cTransform`                | `lili::createCircle(...)`                                 |
| `lili::Line`                                          | `cLine` + `cRender2D` + `cTransform`                  | `lili::createLine(...)`                                   |
| `lili::Text`                                          | `cText` + `cRender2D` + `cTransform`                  | `lili::createText(...)` + `ECSRegistry::getStringArena()` |
| `lili::UILayout`                                      | `cUILayout`                                           | `UILayoutSystem`                                          |
| `lili::AnimationPlayer`                               | `cAnimation`                                          | `AnimationSystem` + `AnimationRegistry`                   |
| `lili::SpriteBatch`                                   | `DynamicBatch` / `StaticBatch`                        | `Render2DSystem` / `BatchHelper`                          |
| `Renderer::drawRect` / `drawDebugRect`                | `cRect`                                               | `lili::createRect(...)`                                   |
| `Renderer::drawCircle` / `drawDebugCircle`            | `cCircle`                                             | `lili::createCircle(...)`                                 |
| `Renderer::drawLine`                                  | `cLine`                                               | `lili::createLine(...)`                                   |
| `Renderer::drawText` / `drawTextId` / `drawDebugText` | `cText`                                               | `lili::createText(...)`                                   |
| `IRenderable::getMaterial()`                          | `cRender2D::material_id`                              | `MaterialRegistry::get()`                                 |

---

### Code Migration Examples

#### 1. Sprites & UI Layout

**Before (OOP):**

```cpp
// In onInit():
sprite = lili::Sprite(getRenderer(), texture);
sprite.setAnchor(lili::Anchor::CENTER);
sprite.setPivot(lili::Pivot::CENTER);
sprite.setOffset({ 0.0f, -50.0f });

// In onRender():
sprite.draw();
```

**After (ECS):**

```cpp
// In onInit():
auto& registry = getECSRegistry();
lili::Entity sprite = lili::createSprite(registry, texture);
registry.addComponent<lili::cUILayout>(
    sprite,
    lili::cUILayout{
        .offset = { 0.0f, -50.0f },
        .anchor = lili::Anchor::CENTER,
        .pivot = lili::Pivot::CENTER
    }
);
// Automatically updated by UILayoutSystem and drawn by Render2DSystem!
```

#### 2. Shapes (Rect, Circle, Line)

**Before (OOP):**

```cpp
circle = lili::Circle(
    getRenderer(),
    lili::CircleShape({ 400.0f, 600.0f }, 60.0f, 32),
    lili::Vec4(0.2f, 0.6f, 1.0f, 1.0f)
);
// Or immediate cached draw:
getRenderer()->drawRect({ 50.0f, 50.0f, 120.0f, 40.0f }, color, /*hollow=*/true);
```

**After (ECS):**

```cpp
auto& registry = getECSRegistry();

lili::Entity circle = lili::createCircle(
    registry,
    { .position = { 400.0f, 600.0f },
      .radius = 60.0f,
      .color = { 0.2f, 0.6f, 1.0f, 1.0f },
      .segments = 32 }
);

lili::Entity rect = lili::createRect(
    registry,
    { .position = { 50.0f, 50.0f },
      .size = { 120.0f, 40.0f },
      .color = color,
      .hollow = true }
);
```

#### 3. Text & Dynamic String Updates (`StringArena`)

**Before (OOP):**

```cpp
fps_text = lili::Text(getRenderer(), font, "FPS: 60");
fps_text.setPosition({ 10.0f, 10.0f });
// In onUpdate():
fps_text.setText("FPS: " + std::to_string(fps));
```

**After (ECS):**

```cpp
// In onInit():
auto& registry = getECSRegistry();
fps_entity = lili::createText(
    registry,
    "FPS: 60",
    { .font = font, .position = { 10.0f, 10.0f } }
);

// In onUpdate():
auto& text_comp = registry.getComponent<lili::cText>(fps_entity);
registry.getStringArena().formatAt(
    text_comp.offset, text_comp.length, "FPS: {}", fps
);
```

#### 4. Animated Sprites

**Before (OOP):**

```cpp
uint16_t anim_id = lili::AnimationRegistry::get().registerAnimation("run", std::move(anim));
anim_sprite = lili::AnimatedSprite(getRenderer(), anim_id);
// In onUpdate(dt):
anim_sprite.update(dt);
// In onRender():
anim_sprite.draw();
```

**After (ECS):**

```cpp
uint16_t anim_id = lili::AnimationRegistry::get().registerAnimation("run", std::move(anim));
lili::Entity player = lili::createAnimatedSprite(
    getECSRegistry(),
    anim_id,
    { .position = { 200.0f, 200.0f }, .scale = { 4.0f, 4.0f } }
);
// Automatically advanced by AnimationSystem and rendered by Render2DSystem!
```

#### 5. Custom Shaders & Materials (`MaterialRegistry`)

**Before (OOP):**

```cpp
rect.setPipeline(custom_pipeline);
rect.getMaterial()->setFragmentUniforms(my_uniforms);
```

**After (ECS):**

```cpp
uint16_t mat_id = lili::MaterialRegistry::get().createMaterial("custom_mat", custom_pipeline);
lili::Entity rect = lili::createRect(
    getECSRegistry(),
    { .position = { 100.0f, 100.0f },
      .size = { 200.0f, 200.0f },
      .material_id = mat_id }
);

// Update uniforms anytime:
lili::MaterialRegistry::get().getMaterial(mat_id)->setFragmentUniforms(my_uniforms);
```
