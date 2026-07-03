#!/bin/bash
if [ "$(basename "$PWD")" = "hello_ecs" ]; then
	cmake -B build
	cmake --build build
else
	echo "Must be executed from hello_ecs/ directory"
fi
