#!/bin/bash
if [ "$(basename "$PWD")" = "hello_scenes" ]; then
	./build/hello_scenes
else
	echo "Must be executed from hello_scenes/ directory"
fi
