#!/bin/bash
if [ "$(basename "$PWD")" = "hello_text" ]; then
	./build/hello_text
else
	echo "Must be executed from hello_text/ directory"
fi
