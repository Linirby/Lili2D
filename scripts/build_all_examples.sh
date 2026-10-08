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

echo -e "${BLUE}Lili2D: Build All Examples${NC}"
echo -e "Repository root: ${YELLOW}$REPO_ROOT${NC}"
echo -e "Examples dir:    ${YELLOW}$EXAMPLES_DIR${NC}"
echo -e "Parallel jobs:   ${YELLOW}$BUILD_JOBS${NC}"
echo ""

if [ -d "$REPO_ROOT/build" ]; then
    echo -e "${BLUE}[1/2] Ensuring Lili2D library is up-to-date...${NC}"
    cmake --build "$REPO_ROOT/build" -j"$BUILD_JOBS" || {
        echo -e "${RED}Failed to build main Lili2D library!${NC}"
        exit 1
    }
fi

echo ""
echo -e "${BLUE}[2/2] Building individual examples...${NC}"

PASSED_COUNT=0
FAILED_COUNT=0
FAILED_EXAMPLES=()
PASSED_EXAMPLES=()

for dir in "$EXAMPLES_DIR"/*/; do
    [ -d "$dir" ] || continue
    example_name=$(basename "$dir")

    [ -f "$dir/CMakeLists.txt" ] || continue

    echo -ne "Building ${YELLOW}$example_name${NC}... "

    mkdir -p "$dir/build"

    if cmake -B "$dir/build" -S "$dir" -DCMAKE_PREFIX_PATH="$REPO_ROOT/build" >"$dir/build/cmake_build.log" 2>&1 &&
        cmake --build "$dir/build" -j"$BUILD_JOBS" >>"$dir/build/cmake_build.log" 2>&1; then
        echo -e "${GREEN}[OK]${NC}"
        PASSED_COUNT=$((PASSED_COUNT + 1))
        PASSED_EXAMPLES+=("$example_name")
    else
        echo -e "${RED}[FAILED]${NC} (see examples/$example_name/build/cmake_build.log)"
        FAILED_COUNT=$((FAILED_COUNT + 1))
        FAILED_EXAMPLES+=("$example_name")
    fi
done

echo ""
echo -e "${BLUE}Summary${NC}"
echo -e "Passed: ${GREEN}$PASSED_COUNT${NC}"
echo -e "Failed: ${RED}$FAILED_COUNT${NC}"

if [ $FAILED_COUNT -gt 0 ]; then
    echo ""
    echo -e "${RED}Failed examples:${NC}"
    for failed in "${FAILED_EXAMPLES[@]}"; do
        echo -e "- ${RED}$failed${NC} (log: examples/$failed/build/cmake_build.log)"
    done
    echo ""
    exit 1
else
    echo ""
    echo -e "${GREEN}All examples built successfully! :3${NC}"
    exit 0
fi
