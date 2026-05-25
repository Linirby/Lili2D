#!/bin/bash
if [ "$(basename "$PWD")" = "hello_rect" ]; then
	./build/hello_rect
else
	echo "Must be executed from hello_rect/ directory"
fi
