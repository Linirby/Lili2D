#!/bin/bash
set -e

echo "Building Lili2D..."
cmake -B build -DCMAKE_BUILD_TYPE=Release

echo "Compiling..."
cmake --build build --parallel

echo "Installing (may require sudo)..."
sudo cmake --install build

echo ""
echo "Done! You can now use Lili2D in your projects with:"
echo ""
echo "  find_package(Lili2D REQUIRED)"
echo "  target_link_libraries(MyGame PRIVATE lili::Lili2D)"
