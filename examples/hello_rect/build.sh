#!/bin/bash
if [ "$(basename "$PWD")" = "hello_rect" ]; then
	cmake -B build
	cmake --build build
else
	echo "Must be executed from hello_rect/ directory"
fi
