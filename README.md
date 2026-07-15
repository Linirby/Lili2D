# Lili2D

![C++20](https://img.shields.io/badge/C++-20-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.10+-brightgreen.svg)
![SDL3](https://img.shields.io/badge/SDL3-Powered-orange.svg)

**Lili2D** is a lightweight, easy-to-use C++ library designed to help you create
2D games (Thanks to SDL_GPU :3)

Whether you are prototyping a quick idea, participating in a game jam, or
building a full 2D game, Lili2D provides you with the modern tools you need to
get things running on screen.

https://github.com/user-attachments/assets/352365ba-e0da-4004-91dd-b28a029c2b4e

## Technical Showcase

For technical leads, developers, and recruiters interested in the engine's
internals (such as the C++20 thread pool, prioritized task scheduler,
cache-friendly ECS design, frustum culling, and command buffering), check out
the **[Technical & Architectural Showcase](docs/ARCHITECTURE.md)**.

## Repository Structure

Lili2D follows the modern C++ Pitchfork layout:

- **[`include/lili2d/`](include/lili2d/)**: The public API headers for the
engine.
- **[`src/`](src/)**: The private implementation files and internal headers.
- **[`examples/`](examples/)**: Example projects demonstrating how to use
Lili2D.
- **[`docs/`](docs/)**: API documentation and Doxygen configuration.
- **[`assets/`](assets/)**: Engine-level default assets.
- **[`scripts/`](scripts/)**: Build and installation utility scripts.

## Getting Started

### Prerequisites

To build and use Lili2D, you will need:

- A **C++20** compatible compiler (GCC, Clang, or MSVC)
- **CMake** 3.10 or higher
- **SDL3** and **SDL3_image**
- **glslc** (Google's shader compiler for compiling custom Vulkan shaders)

### Building from Source

Lili2D uses CMake. You can build and install it globally or include it in your
project's subdirectories.

```bash
git clone https://github.com/Linirby/Lili2D.git
cd Lili2D
mkdir build && cd build
cmake ..
make -j$(nproc)
sudo make install
```

### Generating Documentation

You can generate the HTML documentation for the API by running Doxygen in the
`docs/` directory:

```bash
cd docs
doxygen Doxyfile
```

## Quick Start: Hello Shapes

Here is how simple it is to get a window open and draw shapes with Lili2D!

**1. Create your application class:**

```cpp
#include <lili2d/lili2d.hpp>
#include <memory>

class App : public lili::Game {
public:
    App() : lili::Game("Hello Lili2D :3", 800, 800) {}

    void onInit() override {
        // Create some cool shapes!
        lili::Renderer *renderer = getRenderer();
        line = lili::Line(
            renderer,
            lili::LineShape({ 50.0f, 50.0f }, { 100.0f, 300.0f }, 1.0f),
            lili::Vec4(0.0f, 1.0f, 0.0f, 1.0f)
        );
        rect = lili::Rect(
            renderer,
            lili::RectShape(350.0f, 375.0f, 100.0f, 50.0f),
            lili::Vec4(1.0f, 0.0f, 0.0f, 1.0f)
        );
        circle = lili::Circle(
            renderer,
            lili::CircleShape({ 400.0f, 100.0f }, 50.0f, 32.0f),
            lili::Vec4(0.0f, 0.0f, 1.0f, 1.0f)
        );
    }

    void onRender(float alpha) override {
        (void)alpha;
        line.draw();
        rect.draw();
        circle.draw();
    }

private:
    lili::Line line;
    lili::Rect rect;
    lili::Circle circle;
};
```

**2. Run your app:**

```cpp
#include <iostream>

int main() {
    App app;
    try {
        app.run();
    } catch (std::exception &e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
```

Check out the `examples/` directory in the repository for more details and
complete projects!

### Available Examples

- **`hello_shapes`**: Demonstrates the basics of window creation and rendering
simples colored shapes.
- **`hello_sprite`**: Learn how to initialize a window, load a texture, and
render a basic sprite.
- **`hello_text`**: See how to load a bitmap font and render dynamic text on the
screen.
- **`hello_camera`**: Show how to use camera and different render layer (WORLD2D
/ UI)
- **`hello_layer`**: Demonstrates the use of rendering layers to control the
drawing order of sprites and shapes.
- **`hello_scenes`**: Demonstrates the use of scenes with its manager.
- **`hello_animation`**: An example of how to load an AtlasMap and create
Animations from it.
- **`hello_shader`**: Demonstrates how to write, compile, and load custom vertex
and fragment shaders.
- **`hello_collision`**: A new example showing the v1 implementation of the AABB
collision system and how it interacts with different shapes.
- **`hello_sprite_batch`**: An example of how to use SpriteBatch with a big map.
- **`hello_tilemap`**: Demonstrates the TileMap and Chunk system for rendering
optimized grid worlds.
- **`hello_ecs`**: Demonstrates the lightweight Entity Component System (ECS) by
spawning and animating thousands of ball entities.
- **`hello_multithreading`**: Demonstrates C++20 multithreaded systems, dynamic
thread allocation, and parallel ECS execution using the prioritized ThreadPool.

## Join the Community

Whether you want to follow the daily development logs, ask questions about
integrating the library, or share the cool 2D games you are building, we'd love
to have you!

Join **Lili's | Dev Lounge** on Discord:
**[https://discord.gg/6S6HyKWgK3](https://discord.gg/6S6HyKWgK3)**

## Support the Project

If you like Lili2D and want to support its development, you can buy me a coffee!
Your support helps me spend more time making this library even cooler. :3

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/liliowo)

You can support me here:
**[https://ko-fi.com/liliowo](https://ko-fi.com/liliowo)** ❤️
