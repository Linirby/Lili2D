# Examples

This directory contains example projects demonstrating how to use the **Lili2D** engine.

## Available Examples

- **`hello_rect`**: Demonstrates the basics of window creation and rendering simple colored rectangles.
- **`hello_sprite`**: Shows how to load a texture and render a 2D sprite to the screen.
- **`hello_text`**: Explains how to load bitmap fonts and render dynamic text.

## Build and run examples

Each example folder has a `src/` folder containing the example code, and a `CMakeLists.txt` demonstrating how to link the Lili2D library to your own projects. 

To easily build and run the examples (on Linux), each folder contains three bash scripts:

- `run.sh` to run the built example
- `build.sh` to build the example
- `build_and_run.sh` to do both

These bash scripts need to be executed from the root of the specific example folder (where the scripts are located).

## Note on Smart Pointers

These examples use the `std::unique_ptr` smart pointer for memory management. If you are not familiar with them, don't panic!

Here are some basic rules to easily replace smart pointers (`unique_ptr`, `shared_ptr`, `weak_ptr`) with raw pointers (`*`), either in your head or in the code:

1. Replace every `var = std::make_unique<type>(args);` with `var = new type(args);`. Make sure to find where this variable goes out of scope and put `delete var;` right before that happens.
2. Replace every `std::unique_ptr<type> var;` with `type *var;`.
3. Most of the time, if a `unique_ptr` is in the `App` class, the `delete` for its raw pointer equivalent could be placed after the `while (running)` loop in `App::run()`.
4. Using `.get()` on a `unique_ptr` retrieves the underlying raw pointer. If you are already using a raw pointer `*`, you can just remove the `.get()`.
