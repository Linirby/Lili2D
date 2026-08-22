 #!/bin/bash
set -e

echo "Building Lili2D in Release mode..."
mkdir -p build_release
cd build_release
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
cd ..

echo "Creating distribution folder..."
DIST_DIR="Lili2D-linux-x86_64"
rm -rf $DIST_DIR

echo "Installing via CMake to distribution folder..."
 cmake --install build_release --prefix $DIST_DIR

echo "Compressing..."
tar -czvf $DIST_DIR.tar.gz $DIST_DIR/

rm -rf $DIST_DIR
echo "Done! Upload $DIST_DIR.tar.gz to GitHub."
