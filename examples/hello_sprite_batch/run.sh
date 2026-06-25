#!/bin/bash
if [ "$(basename "$PWD")" = "hello_sprite_batch" ]; then
	./build/hello_sprite_batch
else
	echo "Must be executed from hello_sprite_batch/ directory"
fi
