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
mkdir -p $DIST_DIR/lib
mkdir -p $DIST_DIR/include

echo "Copying binaries..."
cp build_release/libLili2D.so $DIST_DIR/lib/

echo "Copying headers..."
cp -r include/lili2d $DIST_DIR/include/

echo "Compressing..."
tar -czvf $DIST_DIR.tar.gz $DIST_DIR/

rm -rf $DIST_DIR
echo "Done! Upload $DIST_DIR.tar.gz to GitHub."
