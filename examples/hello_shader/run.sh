#!/bin/bash
if [ "$(basename "$PWD")" = "hello_shader" ]; then
	./build/hello_shader
else
	echo "Must be executed from hello_shader/ directory"
fi
