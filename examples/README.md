# Examples

- **`hello_animation`**: Sprite sheet animation (`AtlasMap`, `AnimationComponent`).
- **`hello_camera`**: Camera viewports, zoom, and render layers (`WORLD2D` vs `UI`).
- **`hello_collision`**: Collision checks (`AABB2`, `CircleCollider`).
- **`hello_ecs`**: Entity Component System queries.
- **`hello_layer`**: Render layer sorting and draw order.
- **`hello_multithreading`**: Task scheduling via `ThreadPool`.
- **`hello_render2d`**: Show the use of the hidden `Render2DSystem` in `Game` class.
- **`hello_scenes`**: Scene stack management and transitions.
- **`hello_shader`**: Custom SPIR-V vertex and fragment shaders.
- **`hello_shapes`**: Basic 2D shapes (`Line`, `Rect`, `Circle`).
- **`hello_sprite`**: Texture loading and 2D sprite transforms.
- **`hello_text`**: Bitmap fonts and text alignment.

## Build and Run

From the root directory of any example:

```sh
cmake -B build && cmake --build build
```

Or using the helper script (automatically builds and runs):

```sh
./run.sh
```

Or build all examples at once from this folder:

```sh
./build_all.sh
```

Or run all examples sequentially for testing (advances to next when window closes):

```sh
./run_all.sh
```
