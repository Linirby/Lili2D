#!/bin/bash
if [ "$(basename "$PWD")" = "hello_multithreading" ]; then
	./build/hello_multithreading
else
	echo "Must be executed from hello_multithreading/ directory"
fi
