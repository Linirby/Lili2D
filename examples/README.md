# Examples

This directory contains example projects demonstrating how to use the **Lili2D**
engine.

## Available Examples

- **`hello_shapes`**: Demonstrates the basics of window creation and rendering
simples colored shapes.
- **`hello_sprite`**: Shows how to load a texture and render a 2D sprite to the
screen.
- **`hello_text`**: Explains how to load bitmap fonts and render dynamic text.
- **`hello_camera`**: Show how to use camera and different render layer (WORLD2D
/ UI).
- **`hello_layer`**: Demonstrates the use of rendering layers to control the
drawing order of sprites and shapes.
- `hello_animation`: An example of how to load an AtlasMap and create Animations
from it.

## Build and run examples

Each example folder has a `src/` folder containing the example code, and a
`CMakeLists.txt` demonstrating how to link the Lili2D library to your own
projects.

To easily build and run the examples (on Linux), each folder contains three bash
scripts:

- `run.sh` to run the built example
- `build.sh` to build the example
- `build_and_run.sh` to do both

These bash scripts need to be executed from the root of the specific example
folder (where the scripts are located).

## Memory Management Note

These examples demonstrate a modern, safe C++ approach to memory management
without manual `new`/`delete`:

1. **Core Systems & Assets (`Window`, `Renderer`, `BitmapFont`, `Texture`)**:
These are "heavy" objects that manage external resources (like SDL windows or
GPU buffers). They are stored using `std::unique_ptr` in the `App` class. This
safely pins them in memory and ensures their resources are automatically freed
when the app closes.

2. **Gameplay Objects (like `Rect`, `Line`, `Sprite`, `Text`)**:
These are stored directly on the stack as standard variables (e.g.,
`lili::Rect rect;`). This is fast, clean, and avoids unnecessary heap
allocations. Under the hood, these shapes manage their own meshes and materials
using `unique_ptr`s, so you never have to worry about memory leaks!

If you are unfamiliar with `std::unique_ptr`, you can think of it as an
automatic raw pointer (`*`). To access its methods, you use `->` (like
`renderer->begin_frame()`), and you can retrieve the raw pointer using `.get()`
if another function requires it (like `renderer.get()`).
