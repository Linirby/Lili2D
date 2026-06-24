#!/bin/bash
if [ "$(basename "$PWD")" = "hello_scenes" ]; then
	./build.sh && ./run.sh
else
	echo "Must be executed from hello_scenes/ directory"
fi
