#!/bin/bash
if [ "$(basename "$PWD")" = "hello_layer" ]; then
	./build/hello_layer
else
	echo "Must be executed from hello_layer/ directory"
fi
