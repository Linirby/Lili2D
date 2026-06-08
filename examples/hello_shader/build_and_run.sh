#!/bin/bash
if [ "$(basename "$PWD")" = "hello_shader" ]; then
	./build.sh && ./run.sh
else
	echo "Must be executed from hello_shader/ directory"
fi
