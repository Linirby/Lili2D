# Lili2D

![C++20](https://img.shields.io/badge/C++-20-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.10+-brightgreen.svg)
![SDL3](https://img.shields.io/badge/SDL3-Powered-orange.svg)

**Lili2D** is a modern, lightweight, high-performance C++20 2D game engine built on top of **SDL3** and modern hardware graphics APIs (via `SDL_GPU`).

Whether you are prototyping a quick game jam idea, building full 2D games, or exploring game engine architecture, Lili2D provides clean abstractions, high CPU/GPU performance, and modern developer ergonomics.

https://github.com/user-attachments/assets/352365ba-e0da-4004-91dd-b28a029c2b4e

## Key Features

- **Modern `SDL_GPU` Renderer**: Next-gen hardware-accelerated rendering pipeline with support for custom shaders, materials, layers, and automated texture batching (`SpriteBatch`).
- **Unified Asset & Resource Management**: Scoped lifecycle management (`unloadScope`), string-keyed caching, custom asset loaders, and **live filesystem hot-reloading** (`Assets` / `ResourceManager<T>`).
- **Virtual Logical Resolution & Scaling**: Automatic letterboxing, viewport scale handling, and seamless physical-to-logical screen coordinate conversion (`toLogicalCoords`).
- **UI Layout System**: Comprehensive anchor alignment (`Anchor`), element bounding pivot positioning (`Pivot`), pixel offsets, and inverse-matrix point containment testing (`containsPoint`).
- **Data-Oriented Entity Component System (ECS)**: Cache-friendly contiguous memory pools (`ComponentPool<T>`) and a thread-safe deferred `CommandBuffer` for lock-free parallel execution.
- **Priority-Scheduled Multithreading**: C++20 `ThreadPool` using `std::jthread` with priority queues (`HIGH`, `NORMAL`, `LOW`) for frame-critical and background workloads.
- **Spatial Physics & Collision Queries**: Axis-Aligned Bounding Boxes (`AABB2`), `CircleCollider`, line segment intersections, containment testing, and zero-allocation debug rendering.
- **TileMap & World Chunking**: Frustum viewport culling and dynamic rebuild budgeting (max 8 chunk rebuilds per frame) for butter-smooth camera movement in massive game worlds.

## Technical Showcase

For technical leads, CTOs, senior developers, and recruiters interested in the engine's internal mechanics (such as lock-free ECS command buffering, C++20 thread pool priorities, matrix transform pipelines, and Vulkan/Direct3D GPU budgeting), check out the **[Technical & Architectural Showcase](docs/ARCHITECTURE.md)**.

## Repository Structure

Lili2D follows the modern C++ Pitchfork layout:

- **[`include/lili2d/`](include/lili2d/)**: Public API headers for the engine.
- **[`src/`](src/)**: Private implementation files and internal rendering/ECS routines.
- **[`examples/`](examples/)**: Standalone example applications showcasing engine systems.
- **[`docs/`](docs/)**: Architecture deep dives and Doxygen configuration.
- **[`assets/`](assets/)**: Default engine assets (shaders, fonts, textures).
- **[`scripts/`](scripts/)**: Build and utility scripts.

## Getting Started

### Prerequisites

To build and use Lili2D, you need:

- A **C++20** compatible compiler (GCC 10+, Clang 11+, or MSVC 2019+)
- **CMake** 3.10 or higher
- **SDL3** and **SDL3_image**
- **glslc** (Google's SPIR-V shader compiler for Vulkan shaders)

### Building from Source

Lili2D uses CMake for building and installation. You can build it standalone or include it as a CMake subdirectory in your game project.

```bash
git clone https://github.com/Linirby/Lili2D.git
cd Lili2D
mkdir build && cd build
cmake ..
make -j$(nproc)
sudo make install
```

### Generating Documentation

Generate HTML API documentation using Doxygen:

```bash
cd docs
doxygen Doxyfile
```

## Quick Start: Hello Shapes & Assets

Here is how simple it is to initialize a window, load assets, position UI elements with anchors/pivots, and render shapes with Lili2D!

**1. Create your application class:**

```cpp
#include <lili2d/lili2d.hpp>
#include <memory>

class App : public lili::Game {
public:
    App() : lili::Game("Hello Lili2D :3", 800, 800) {}

    void onInit() override {
        // Set virtual logical resolution (automatic aspect ratio scaling)
        getWindow()->setLogicalResolution(800, 800);

        // Load assets using the static Assets facade
        lili::Texture* cat_tex = lili::Assets::loadTexture(
            "cat", "assets/textures/cat.png", getRenderer()->getDevice()
        );

        // Create a sprite with UI anchor & pivot positioning
        sprite = lili::Sprite(getRenderer(), cat_tex);
        sprite.setAnchor(lili::Anchor::Center);
        sprite.setPivot(lili::Pivot::Center);
        sprite.setOffset({ 0.0f, -50.0f });

        // Create vector shapes
        circle = lili::Circle(
            getRenderer(),
            lili::CircleShape({ 400.0f, 600.0f }, 60.0f, 32.0f),
            lili::Vec4(0.2f, 0.6f, 1.0f, 1.0f)
        );
    }

    void onRender(float alpha) override {
        (void)alpha;
        sprite.draw();
        circle.draw();
    }

private:
    lili::Sprite sprite;
    lili::Circle circle;
};

int main() {
    App app;
    app.run();
    return 0;
}
```

### Available Examples

Check out the [`examples/`](examples/) directory for comprehensive code samples:

- **`hello_shapes`**: Window initialization and 2D shape rendering (`Line`, `Rect`, `Circle`).
- **`hello_sprite`**: Texture loading using `lili::Assets` and 2D sprite transforms.
- **`hello_text`**: Bitmap font rendering and text alignment.
- **`hello_camera`**: Camera viewports, zoom factors, and rendering layers (`WORLD2D` vs `UI`).
- **`hello_layer`**: Render layer sorting and draw-order management.
- **`hello_scenes`**: Scene stack management and smooth state transitions.
- **`hello_animation`**: Loading sprite sheets (`AtlasMap`) and playing frame animations.
- **`hello_shader`**: Writing, compiling, and binding custom SPIR-V vertex and fragment shaders.
- **`hello_collision`**: AABB and `CircleCollider` spatial collision testing and debug drawing.
- **`hello_sprite_batch`**: High-performance batch rendering of thousands of sprites in a single GPU call.
- **`hello_tilemap`**: Optimized grid tilemaps, chunking, and viewport frustum culling.
- **`hello_ecs`**: High-entity-count physics and animation driven by the Data-Oriented ECS.
- **`hello_multithreading`**: Priority multithreaded task scheduling and parallel ECS updates via `ThreadPool`.

## Join the Community

Whether you want to follow development logs, ask integration questions, or share your game projects:

Join **Lili's | Dev Lounge** on Discord:  
👉 **[https://discord.gg/6S6HyKWgK3](https://discord.gg/6S6HyKWgK3)**

## Support the Project

If you enjoy using Lili2D and want to support its ongoing development:

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/liliowo)  
👉 **[https://ko-fi.com/liliowo](https://ko-fi.com/liliowo)** ❤️

