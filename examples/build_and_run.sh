#!/bin/bash
if [ "$(basename "$PWD")" = "examples" ]; then
	./build.sh && ./run.sh
else
	echo "Must be executed from examples/ directory"
fi
