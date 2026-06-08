#!/bin/bash
if [ "$(basename "$PWD")" = "hello_shader" ]; then
	cmake -B build
	cmake --build build
else
	echo "Must be executed from hello_shader/ directory"
fi
