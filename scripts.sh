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
    if [ "$1" == "dev" ]; then
        echo -e "\n--- Running Dev Environment ---"
        ./build/BlipDev
    else
        echo -e "\n--- Starting Blip ---"
        ./build/Blip
    fi
fi
