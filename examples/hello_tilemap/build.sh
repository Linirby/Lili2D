#!/bin/bash
if [ "$(basename "$PWD")" = "hello_tilemap" ]; then
	cmake -B build
	cmake --build build
else
	echo "Must be executed from hello_tilemap/ directory"
fi
