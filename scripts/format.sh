#!/bin/bash
git ls-files '*.c' '*.cpp' '*.h' '*.hpp' | xargs clang-format -i
