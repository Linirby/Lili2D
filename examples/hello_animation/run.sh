#!/bin/bash
if [ "$(basename "$PWD")" = "hello_animation" ]; then
	./build/hello_animation
else
	echo "Must be executed from hello_animation/ directory"
fi
