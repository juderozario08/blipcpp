#include <SDL.h>
#include <SDL_video.h>
#include <blip/app/main.hpp>
#include <blip/buffer/table.hpp>
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

void eventLoop(app::AppState &appState, config::ConfigWatcher &watcher, config::EditorConfig &state, buffer::PieceTable &pt) {
    auto running = true;
    SDL_Event event;

    size_t cursor_pos = 0;
    bool dirty = false;

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
                break;
            case SDL_TEXTINPUT:
                pt.insert(cursor_pos, event.text.text);
                cursor_pos += strlen(event.text.text);
                dirty = true;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (cursor_pos > 0) {
                        pt.erase(cursor_pos - 1, 1);
                        cursor_pos--;
                        dirty = true;
                    }
                } else if (event.key.keysym.sym == SDLK_RETURN) {
                    pt.insert(cursor_pos, "\n");
                    cursor_pos++;
                    dirty = true;
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    if (cursor_pos > 0) {
                        cursor_pos--;
                        dirty = true;
                    }
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    if (cursor_pos < pt.length()) {
                        cursor_pos++;
                        dirty = true;
                    }
                }
                break;
            }

            if (dirty) {
                std::cout << "\n--- Blip State ---\n";
                std::cout << "Length: " << pt.length() << " | Cursor: " << cursor_pos << "\n";

                std::string current_text = pt.getText();
                current_text.insert(cursor_pos, "|");
                std::cout << current_text << std::endl;

                dirty = false;
            }
        }

        watcher.check();
        drawBackground(appState, state);
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
    appState.window = SDL_CreateWindow("Blip", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
                                       SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    if (appState.window == NULL) {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    SDL_RaiseWindow(appState.window);

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
    // core::printState(state);// NOTE: THIS FUNCTION IS FOR DEBUGGING ONLY
    watcher.start(filepath, [&state, filepath]() {
        config::loadConfig(filepath, state);
        // core::printState(state);// NOTE: THIS FUNCTION IS FOR DEBUGGING ONLY
    });

    // TODO: Should be the ORIGINAL FILE TEXT HERE. IMPLEMENT LATER.
    buffer::PieceTable pt("Hello World");

    eventLoop(appState, watcher, state, pt);

    SDL_DestroyRenderer(appState.renderer);
    SDL_DestroyWindow(appState.window);
    SDL_Quit();
}

int main() {
    run();
    return 0;
}
