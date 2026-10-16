#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
EXAMPLES_DIR="$REPO_ROOT/examples"
BUILD_JOBS=$(nproc 2>/dev/null || echo 4)

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}Lili2D: Run All Examples :3${NC}"
echo ""

if [ -d "$REPO_ROOT/build" ]; then
    echo -e "${BLUE}[1/2] Ensuring Lili2D library is up-to-date...${NC}"
    cmake --build "$REPO_ROOT/build" -j"$BUILD_JOBS" || {
        echo -e "${RED}Failed to build main Lili2D library!${NC}"
        exit 1
    }
fi

echo ""
echo -e "${BLUE}[2/2] Running examples sequentially... (close window to proceed)${NC}"
echo ""

# Exit cleanly if user presses Ctrl+C
trap 'echo -e "\n${YELLOW}Stopped testing. :3${NC}"; exit 0' INT

for dir in "$EXAMPLES_DIR"/*/; do
    [ -d "$dir" ] || continue
    example_name=$(basename "$dir")

    [ -f "$dir/CMakeLists.txt" ] || continue

    echo -e "- Running ${YELLOW}$example_name${NC}..."

    mkdir -p "$dir/build"

    if ! cmake -B "$dir/build" -S "$dir" -DCMAKE_PREFIX_PATH="$REPO_ROOT/build" -DLili2D_DIR="$REPO_ROOT/build" >"$dir/build/cmake_build.log" 2>&1 || \
       ! cmake --build "$dir/build" -j"$BUILD_JOBS" >>"$dir/build/cmake_build.log" 2>&1; then
        echo -e "${RED}[BUILD FAILED] $example_name${NC} (see examples/$example_name/build/cmake_build.log)"
        continue
    fi

    (cd "$dir/build" && ./"$example_name")
    exit_code=$?

    if [ $exit_code -eq 130 ] || [ $exit_code -eq 2 ]; then
        echo -e "\n${YELLOW}Interrupted. Stopped testing. :3${NC}"
        exit 0
    fi
done

echo ""
echo -e "${GREEN}All examples finished! :3${NC}"
