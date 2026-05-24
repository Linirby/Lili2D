#!/bin/bash
if [ "$(basename "$PWD")" = "hello_text" ]; then
	cmake -B build
	cmake --build build
else
	echo "Must be executed from hello_text/ directory"
fi
