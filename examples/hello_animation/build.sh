#!/bin/bash
if [ "$(basename "$PWD")" = "hello_animation" ]; then
	cmake -B build
	cmake --build build
else
	echo "Must be executed from hello_animation/ directory"
fi
