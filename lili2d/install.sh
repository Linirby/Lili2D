#!/bin/bash
set -e

./build.sh

echo "Installing..."
sudo cmake --install build

echo ""
echo "Done! You can now use Lili2D in your projects with:"
echo ""
echo "  find_package(Lili2D REQUIRED)"
echo "  target_link_libraries(MyGame PRIVATE lili::Lili2D)"
