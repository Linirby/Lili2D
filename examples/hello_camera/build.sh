#!/bin/bash
if [ "$(basename "$PWD")" = "hello_camera" ]; then
	cmake -B build
	cmake --build build
else
	echo "Must be executed from hello_camera/ directory"
fi
