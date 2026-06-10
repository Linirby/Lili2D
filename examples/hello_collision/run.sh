#!/bin/bash
if [ "$(basename "$PWD")" = "hello_collision" ]; then
	./build/hello_collision
else
	echo "Must be executed from hello_collision/ directory"
fi
