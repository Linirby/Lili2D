#!/bin/bash
if [ "$(basename "$PWD")" = "hello_ecs" ]; then
	./build/hello_ecs
else
	echo "Must be executed from hello_ecs/ directory"
fi
