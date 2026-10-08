#!/bin/bash
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
TARGET_NAME="$(basename "$SCRIPT_DIR")"

cmake -B "$SCRIPT_DIR/build" -S "$SCRIPT_DIR" -DCMAKE_PREFIX_PATH="$REPO_ROOT/build"
cmake --build "$SCRIPT_DIR/build"

cd "$SCRIPT_DIR/build" && ./"$TARGET_NAME" "$@"
