#!/bin/bash
if [ "$(basename "$PWD")" = "hello_sprite_batch" ]; then
	cmake -B build
	cmake --build build
else
	echo "Must be executed from hello_sprite_batch/ directory"
fi
