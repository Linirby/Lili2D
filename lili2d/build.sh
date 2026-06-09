#!/bin/bash
set -e

echo "Building Lili2D..."
cmake -B build -DCMAKE_BUILD_TYPE=Release

echo "Compiling..."
cmake --build build --parallel
