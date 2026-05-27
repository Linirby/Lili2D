#!/bin/bash
if [ "$(basename "$PWD")" = "hello_camera" ]; then
	./build/hello_camera
else
	echo "Must be executed from hello_camera/ directory"
fi
