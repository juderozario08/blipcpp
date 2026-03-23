#!/bin/bash

cmake -S . -B build
cmake --build build

if [ ! -f ./compile_commands.json ]; then
    ln -s build/compile_commands.json .
fi

if [ "$1" == "test" ]; then
    echo -e "\n--- Running Tests ---"
    ./build/BlipTests
else
    echo -e "\n--- Starting Blip ---"
    ./build/Blip
fi
