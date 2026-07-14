#!/bin/bash
if [ "$(basename "$PWD")" = "hello_multithreading" ]; then
	# Copy assets
	cp ../hello_ecs/circle.png .
	
	# Build
	cmake -B build
	cmake --build build
else
	echo "Must be executed from hello_multithreading/ directory"
fi
