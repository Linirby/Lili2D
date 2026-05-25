# Lili2D

![C++20](https://img.shields.io/badge/C++-20-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.16+-brightgreen.svg)
![SDL3](https://img.shields.io/badge/SDL3-Powered-orange.svg)

**Lili2D** is a lightweight, easy-to-use C++ library designed to help you create
2D games with a cool, intuitive API! :3

Whether you are prototyping a quick idea, participating in a game jam, or
building a full 2D game, Lili2D provides you with the modern tools you need to
get things running on screen fast.

## Features

- **Cool & Intuitive API**: Modern Object-Oriented design that gets out of your
way.
- **Modern C++20**: Built with modern C++ features for maximum performance and
safety.
- **Rendering & Shaders**: Custom rendering pipeline using SDL3 and compiled
SPIR-V shaders.
- **Core Systems Included**:
  - **Render**: Windows, Sprites, UI Text, Cameras, and Materials.
  - **Geometry**: Vectors and Matrices (Vec2, Vec3, Vec4, Mat3, Mat4).
  - **Physics**: Built-in 2D collision detection.
  - **Events**: Easy event polling, Keyboard, and Mouse input handling.
- **Easy Integration**: Simple CMake integration (`find_package(Lili2D REQUIRED)`).

## Getting Started

### Prerequisites

To build and use Lili2D, you will need:

- A **C++20** compatible compiler (GCC, Clang, or MSVC)
- **CMake** 3.16 or higher
- **SDL3** and **SDL3_image**
- **glslc** (Google's shader compiler for compiling custom Vulkan shaders)

### Building and Installing

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

## Quick Start: Hello Sprite

Here is how simple it is to get a window open and draw a sprite with Lili2D!

**1. Create your application class:**

```cpp
#include <lili/core.hpp>
#include <lili/render.hpp>
#include <memory>

class App {
public:
    App() {
        // Create a resizable window and a renderer
        window = std::make_unique<lili::Window>("Hello Lili2D :3", 800, 600);
        window->set_resizable(true);
        renderer = std::make_unique<lili::Renderer>(window.get());

        // Load a cute cat sprite!
        cat_sprite = std::make_unique<lili::Sprite>(renderer.get(), "cat.png");
        cat_sprite->set_scale({ 0.5f, 0.5f });

        running = true;
    }

    void run() {
        while (running) {
            handle_events();
            render();
        }
    }

private:
    std::unique_ptr<lili::Window> window;
    std::unique_ptr<lili::Renderer> renderer;
    std::unique_ptr<lili::Sprite> cat_sprite;
    bool running;

    void handle_events() {
        lili::Event event;
        while (event.poll()) {
            if (event.type() == lili::EventType::QUIT) {
                running = false;
            }
            if (event.keyboard().key == SDLK_ESCAPE) {
                running = false;
            }
        }
    }

    void render() {
        if (!renderer->begin_frame()) return;

        // Center the sprite in the window
        cat_sprite->set_position({
            window->get_width() / 2.0f, 
            window->get_height() / 2.0f
        });

        cat_sprite->draw();

        renderer->end_frame();
    }
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

### Available Examples:
- **`hello_sprite`**: Learn how to initialize a window, load a texture, and render a basic sprite.
- **`hello_text`**: See how to load a bitmap font and render dynamic text on the screen.

## Support the Project ❤️

If you like Lili2D and want to support its development, you can buy me a coffee!
Your support helps me spend more time making this library even cooler. :3

[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/liliowo)

You can support me here:
**[https://ko-fi.com/liliowo](https://ko-fi.com/liliowo)** ❤️
