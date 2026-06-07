# Lili2D Engine Core

This directory contains the core source code for the **Lili2D** game engine.

Lili2D is designed to be a lightweight, C++20-based 2D game framework that
provides a modern, object-oriented API for rendering, physics, and input handling.

## Directory Structure

- **`src/`**: Contains all the `.cpp` source files and internal implementations.
- **`assets/`**: Default engine assets (e.g., default shaders, fonts, or icons
used internally).
- **`cmake/`**: CMake modules and scripts specific to building the engine.
- **`CMakeLists.txt`**: The build configuration for the engine library itself.

## Engine Architecture

Lili2D is broken down into several core subsystems:

- **Core & Events (`lili/core.hpp`)**: Manages the application lifecycle, window
creation, and event polling (keyboard, mouse, window events).
- **Render (`lili/render.hpp`)**: A custom 2D rendering pipeline powered by SDL3
and Vulkan/SPIR-V shaders. Includes abstractions for:
  - `Renderer`: The main rendering context, supporting custom render layers.
  - `Sprite` & `Texture`: Image loading and drawing, with support for sizing and
  scaling.
  - `Camera`: 2D view manipulations.
  - `Text`: Bitmap font rendering.
- **Geometry & Math**: Provides foundational math types like `Vec2`, `Vec3`,
`Mat3`, and `Mat4`.
- **Physics**: Provides 2D AABB and circle collision detection routines.

## Building the Engine

To build just the engine (without examples), you can run CMake from within this
directory or the root directory.

### Dependencies

Ensure you have the following installed on your system:

- **C++20 Compiler** (GCC, Clang, MSVC)
- **CMake 3.16+**
- **SDL3** and **SDL3_image** (Development libraries)
- **glslc** (Required to compile `.vert` and `.frag` shaders into SPIR-V)

### CMake Build Steps

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

This will produce the `Lili2D` static/shared library depending on your
configuration, which can then be linked into your own projects.

## Documentation

The engine source code is fully documented using Doxygen-style docstrings. To
generate the HTML documentation, run:

```bash
doxygen Doxyfile
```

This will generate the documentation in the `html/` directory which you can view
in your browser.

## Contributing to the Engine

When modifying the engine core:

1. Ensure your changes compile against the C++20 standard.
2. If you add new features, consider adding a corresponding example in the root
`examples/` directory.
3. Keep the API intuitive! If a function name or class design feels clunky, it
probably needs a refactor to maintain Lili2D's easy-to-use philosophy.
