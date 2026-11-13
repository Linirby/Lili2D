#!/bin/bash
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

echo "Cleaning Lili2D build artifacts and generated files..."

rm -rf \
    "$REPO_ROOT/build" \
    "$REPO_ROOT/docs/html" \
    "$REPO_ROOT/.cache" \
    "$REPO_ROOT/Lili2D-linux-x86_64"

rm -f "$REPO_ROOT"/*.tar.gz

for example_build in "$REPO_ROOT"/examples/*/build; do
    if [ -d "$example_build" ]; then
        rm -rf "$example_build"
    fi
done

echo "Done!"
