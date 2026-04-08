cmake_minimum_required(VERSION 3.10)
project(Blip)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(Fontconfig REQUIRED)

set(CORE_SOURCES
    src/config/editor.cpp
    src/core/log.cpp
    src/buffer/table.cpp
    src/buffer/buffer.cpp
    src/text/font_manager.cpp
    src/text/highlighter.cpp
    src/text/typesetter.cpp
    src/ui/renderer.cpp
    src/ui/window.cpp
)

if (APPLE)
    list(APPEND CORE_SOURCES src/platform/darwin/watcher.cpp)
    list(APPEND CORE_SOURCES src/platform/darwin/system.cpp)
elseif (UNIX AND NOT APPLE)
    list(APPEND CORE_SOURCES src/platform/linux/watcher.cpp)
    list(APPEND CORE_SOURCES src/platform/linux/system.cpp)
endif()

# Helper function to avoid repeating the same linking logic
function(add_blip_executable name)
    add_executable(${name} src/app/main.cpp ${CORE_SOURCES})
    target_include_directories(${name} PRIVATE include)
    target_link_libraries(${name}
        PRIVATE
            SDL2::SDL2
            SDL2_ttf::SDL2_ttf
            Fontconfig::Fontconfig
    )
endfunction()

add_blip_executable(Blip)

add_blip_executable(BlipDev)
target_compile_definitions(BlipDev PRIVATE _DEV_)

add_blip_executable(BlipTests)
