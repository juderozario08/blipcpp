cmake_minimum_required(VERSION 3.10)
project(Blip)

# --- LSP & TOOLING ---
# Generates compile_commands.json for clangd/ccls
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Standard C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- DEPENDENCIES ---
# On Arch Linux, ensure 'sdl2' is installed via pacman.
# On Mac, ensure it is installed via Homebrew.
find_package(SDL2 REQUIRED)

# --- SOURCE FILES ---
set(SOURCES
    main.cpp
    config/editor.cpp
    editor/app.cpp
    logging/log.cpp
)

if (APPLE)
    list(APPEND SOURCES config/watcher_darwin.cpp)
elseif (UNIX AND NOT APPLE)
    list(APPEND SOURCES config/watcher_linux.cpp)
endif()

# --- EXECUTABLE ---
add_executable(Blip ${SOURCES})

# --- INCLUDE DIRECTORIES ---
target_include_directories(Blip PRIVATE include)

# --- LINKING ---
# Using the modern CMake target SDL2::SDL2 is most portable
target_link_libraries(Blip PRIVATE SDL2::SDL2)
