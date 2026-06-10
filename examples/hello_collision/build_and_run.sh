#!/bin/bash
if [ "$(basename "$PWD")" = "hello_collision" ]; then
	./build.sh && ./run.sh
else
	echo "Must be executed from hello_collision/ directory"
fi
