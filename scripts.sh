cmake -S . -B build
cmake --build build

if [ ! -f ./compile_commands.json ]; then
    ln -s build/compile_commands.json .
fi

./build/Blip
