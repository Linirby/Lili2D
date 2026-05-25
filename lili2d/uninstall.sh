#!/bin/bash
set -e

echo "Uninstalling Lili2D..."

if [ -f build/install_manifest.txt ]; then
    echo "Found install_manifest.txt, removing installed files..."
    sudo xargs rm -f < build/install_manifest.txt
    sudo rmdir /usr/local/include/lili2d 2>/dev/null || true
    sudo rmdir /usr/local/lib/cmake/Lili2D 2>/dev/null || true
else
    echo "install_manifest.txt not found. Performing manual cleanup..."
    sudo rm -rf /usr/local/include/lili2d
    sudo rm -rf /usr/local/lib/cmake/Lili2D
    sudo rm -f /usr/local/lib/libLili2D.so*
fi

echo "Done!"
