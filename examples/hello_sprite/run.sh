#!/bin/bash
if [ "$(basename "$PWD")" = "hello_sprite" ]; then
	./build/hello_sprite
else
	echo "Must be executed from hello_sprite/ directory"
fi
