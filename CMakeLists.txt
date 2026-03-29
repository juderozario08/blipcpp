cmake_minimum_required(VERSION 3.10)
project(Blip)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()

find_package(SDL2 REQUIRED)

set(CORE_SOURCES
    src/config/editor.cpp
    src/core/log.cpp
    src/buffer/table.cpp
    src/buffer/buffer.cpp
)

if (APPLE)
    list(APPEND CORE_SOURCES src/platform/darwin/watcher.cpp)
elseif (UNIX AND NOT APPLE)
    list(APPEND CORE_SOURCES src/platform/linux/watcher.cpp)
endif()

add_executable(Blip src/app/main.cpp ${CORE_SOURCES})
target_include_directories(Blip PRIVATE include)
target_link_libraries(Blip PRIVATE SDL2::SDL2)

add_executable(BlipTests src/tests/tests.cpp ${CORE_SOURCES})
target_include_directories(BlipTests PRIVATE include)
target_link_libraries(BlipTests PRIVATE SDL2::SDL2)
