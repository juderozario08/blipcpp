#include <SDL.h>
#include <blip/app/main.hpp>
#include <blip/config/editor.hpp>
#include <blip/core/log.hpp>
#include <blip/platform/watcher.hpp>
#include <iostream>

void drawBackground(app::AppState &appState, config::EditorConfig &state) {
    auto c = state.theme.background;
    SDL_SetRenderDrawColor(appState.renderer, c.r, c.g, c.b, c.a);
    SDL_RenderClear(appState.renderer);
}

// TODO: FIGURE THIS OUT LATER
void drawText(app::AppState &appState, config::EditorConfig &state) {
    config::setForegroundColor(appState, state);
    config::setSelectionColor(appState, state);
    config::setCursorColor(appState, state);
}
void drawDiagnostic(app::AppState &appState, config::EditorConfig &state) {
    config::setDiagnosticErrorColor(appState, state);
    config::setDiagnosticInfoColor(appState, state);
    config::setDiagnosticWarningColor(appState, state);
}
void drawGitLogs(app::AppState &appState, config::EditorConfig &state) {
    config::setDiffAddColor(appState, state);
    config::setDiffChangeColor(appState, state);
    config::setDiffRemoveColor(appState, state);
}
void drawState(app::AppState &appState, config::EditorConfig &state) {
    config::setLineNumberColor(appState, state);
    config::setPopupBackgroundColor(appState, state);
    config::setTooltipBorderColor(appState, state);
    config::setCompletionBackgroundColor(appState, state);
    config::setHoverBackgroundColor(appState, state);
}

void eventLoop(app::AppState &appState, config::ConfigWatcher &watcher, config::EditorConfig &state) {
    auto running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    SDL_GetWindowSize(appState.window, &appState.window_width, &appState.window_height);
                }
            }
        }

        watcher.check();
        drawBackground(appState, state);
        drawDiagnostic(appState, state);
        drawState(appState, state);
        drawGitLogs(appState, state);
        drawText(appState, state);
        SDL_RenderPresent(appState.renderer);
    }
}

void run() {
    app::AppState appState;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "SDL Init Error" << std::endl;
        exit(EXIT_FAILURE);
    }
    appState.window = SDL_CreateWindow("Blip", 0, 0, 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS);
    if (appState.window == NULL) {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    SDL_GetWindowSize(appState.window, &appState.window_width, &appState.window_height);
    appState.renderer = SDL_CreateRenderer(appState.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (appState.renderer == NULL) {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    config::EditorConfig state;
    config::setDefaultConifg(state);
    config::ConfigWatcher watcher;

    // TODO: Calculate filepath using system file lookup
    std::string filepath = "config.ini";
    config::loadConfig(filepath, state);
    core::printState(state);
    watcher.start(filepath, [&state, filepath]() {
        config::loadConfig(filepath, state);
        core::printState(state);
    });

    eventLoop(appState, watcher, state);

    SDL_DestroyRenderer(appState.renderer);
    SDL_DestroyWindow(appState.window);
    SDL_Quit();
}

int main() {
    run();
    return 0;
}
