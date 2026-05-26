#!/bin/bash
if [ "$(basename "$PWD")" = "hello_shapes" ]; then
	./build.sh && ./run.sh
else
	echo "Must be executed from hello_shapes/ directory"
fi
