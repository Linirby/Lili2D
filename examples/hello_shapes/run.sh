#!/bin/bash
if [ "$(basename "$PWD")" = "hello_shapes" ]; then
	./build/hello_shapes
else
	echo "Must be executed from hello_shapes/ directory"
fi
