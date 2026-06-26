#!/bin/bash
if [ "$(basename "$PWD")" = "hello_tilemap" ]; then
	./build/hello_tilemap
else
	echo "Must be executed from hello_tilemap/ directory"
fi
