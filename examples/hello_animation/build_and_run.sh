#!/bin/bash
if [ "$(basename "$PWD")" = "hello_animation" ]; then
	./build.sh && ./run.sh
else
	echo "Must be executed from hello_animation/ directory"
fi
