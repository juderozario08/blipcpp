#pragma once
#include <SDL.h>
namespace editor {
typedef struct AppState {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int window_width;
    int window_height;
} AppState;
void run();
}
