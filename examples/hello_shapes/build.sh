#!/bin/bash
if [ "$(basename "$PWD")" = "hello_shapes" ]; then
	cmake -B build
	cmake --build build
else
	echo "Must be executed from hello_shapes/ directory"
fi
