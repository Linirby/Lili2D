#!/bin/bash
set -e

# Build the Docker image
echo "Building the Docker image 'lili2d-builder:latest'..."
docker build -t lili2d-builder:latest -f Dockerfile .

echo "Running the build inside the container..."
docker run --rm \
    -u "$(id -u):$(id -g)" \
    -v "$PWD:/workspace" \
    -w /workspace \
    lili2d-builder:latest \
    bash -c '
set -e
echo "Building Lili2D in Release mode..."
rm -rf build_docker
mkdir -p build_docker
cd build_docker
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
cd ..

echo "Creating distribution folder..."
DIST_DIR="Lili2D-linux-x86_64"
rm -rf $DIST_DIR

echo "Installing via CMake to distribution folder..."
cmake --install build_docker --prefix $DIST_DIR

echo "Compressing..."
tar -czvf $DIST_DIR.tar.gz $DIST_DIR/

rm -rf $DIST_DIR
echo "Done! Final distribution artifact $DIST_DIR.tar.gz is ready to be uploaded to GitHub."
'
