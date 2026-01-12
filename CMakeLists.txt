cmake_minimum_required(VERSION 3.10)
project(Blip)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

set(SOURCES
    src/app/main.cpp
    src/config/editor.cpp
    src/core/log.cpp
    src/tests/tests.cpp
)

if (APPLE)
    list(APPEND SOURCES src/platform/darwin/watcher.cpp)
elseif (UNIX AND NOT APPLE)
    list(APPEND SOURCES src/platform/linux/watcher.cpp)
endif()

add_executable(Blip ${SOURCES})

target_include_directories(Blip PRIVATE include)
target_link_libraries(Blip PRIVATE SDL2::SDL2)
