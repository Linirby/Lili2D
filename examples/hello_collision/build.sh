#!/bin/bash
if [ "$(basename "$PWD")" = "hello_collision" ]; then
	cmake -B build
	cmake --build build
else
	echo "Must be executed from hello_collision/ directory"
fi
