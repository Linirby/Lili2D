#!/bin/bash
if [ "$(basename "$PWD")" = "examples" ]; then
	./build/hello_sprite
else
	echo "Must be executed from examples/ directory"
fi
