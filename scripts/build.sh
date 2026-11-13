#!/bin/bash
set -e

echo "Building Lili2D..."
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DLILI2D_BUILD_TESTS=ON

echo "Compiling..."
cmake --build build --target lili2d_tests -j$(nproc)
ctest --test-dir build --output-on-failure
