# Examples

- **`hello_shapes`**: Basic 2D shapes (`Line`, `Rect`, `Circle`).
- **`hello_sprite`**: Texture loading and 2D sprite transforms.
- **`hello_text`**: Bitmap fonts and text alignment.
- **`hello_camera`**: Camera viewports, zoom, and render layers (`WORLD2D` vs `UI`).
- **`hello_layer`**: Render layer sorting and draw order.
- **`hello_scenes`**: Scene stack management and transitions.
- **`hello_animation`**: Sprite sheet animation (`AtlasMap`, `AnimationPlayer`).
- **`hello_shader`**: Custom SPIR-V vertex and fragment shaders.
- **`hello_collision`**: Collision checks (`AABB2`, `CircleCollider`).
- **`hello_sprite_batch`**: Batch rendering sprites in a single draw call.
- **`hello_tilemap`**: Tilemap chunks with frustum culling.
- **`hello_ecs`**: Entity Component System queries.
- **`hello_multithreading`**: Task scheduling via `ThreadPool`.

## Build and Run

From the root directory of any example:

```sh
cmake -B build && cmake --build build
```

Or using the helper scripts:

```sh
sh build.sh && sh run.sh
```
