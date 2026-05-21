#!/bin/bash
if [ "$(basename "$PWD")" = "examples" ]; then
	make -C shader/
	cmake -B build
	cmake --build build
else
	echo "Must be executed from examples/ directory"
fi
