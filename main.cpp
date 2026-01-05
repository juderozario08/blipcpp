#include <SDL.h>
#include <config/config_watcher.hpp>
#include <config/editor_config.hpp>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

void drawBackground(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void drawCentreRect(SDL_Renderer *renderer, SDL_Window *window, config::EditorConfig *state) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    int boxWidth = (w * 5) / 100, boxHeight = (h * 5) / 100;
    int middleX = w / 2 - (boxWidth / 2), middleY = h / 2 - (boxHeight / 2);
    SDL_Rect rect = {.x = middleX, .y = middleY, .w = boxWidth, .h = boxHeight};
    SDL_RenderFillRect(renderer, &rect);
}

template <typename T> void printVal(T value, const char *str) {
    if constexpr (std::is_same_v<T, config::Color>) {
        printf("    %s = rgba(%u,%u,%u,%u)\n", str, value.r, value.g, value.b, value.a);
    } else if constexpr (std::is_same_v<T, std::vector<std::string>>) {
        if (value.size() <= 0) {
            return;
        }
        std::cout << "    " << str << "= [" << std::endl;
        for (const auto &el : value) {
            std::cout << "\t" << el << std::endl;
        }
        std::cout << "    ]" << std::endl;
    } else if constexpr (std::is_same_v<Uint8, T> || std::is_same_v<Uint16, T>) {
        printf("    %s = %u\n", str, value);
    } else {
        std::cout << "    " << str << " = " << value << std::endl;
    }
}

void printState(config::EditorConfig *state) {
    std::cout << "[Theme]" << std::endl;
    printVal(state->theme.background, config::constants::theme::BACKGROUND);
    printVal(state->theme.foreground, config::constants::theme::FOREGROUND);
    printVal(state->theme.cursor, config::constants::theme::CURSOR);
    printVal(state->theme.selection, config::constants::theme::SELECTION);
    printVal(state->theme.line_number, config::constants::theme::LINE_NUMBER);
    printVal(state->theme.whitespace, config::constants::theme::WHITESPACE);
    printVal(state->theme.diff_add, config::constants::theme::DIFF_ADD);
    printVal(state->theme.diff_remove, config::constants::theme::DIFF_REMOVE);
    printVal(state->theme.diff_change, config::constants::theme::DIFF_CHANGE);
    printVal(state->theme.diagnostic_error, config::constants::theme::DIAGNOSTIC_ERROR);
    printVal(state->theme.diagnostic_warning, config::constants::theme::DIAGNOSTIC_WARNING);
    printVal(state->theme.diagnostic_info, config::constants::theme::DIAGNOSTIC_INFO);
    printVal(state->theme.popup_background, config::constants::theme::POPUP_BACKGROUND);
    printVal(state->theme.tooltip_border, config::constants::theme::TOOLTIP_BORDER);
    printVal(state->theme.completion_background, config::constants::theme::COMPLETION_BACKGROUND);
    printVal(state->theme.hover_tab_background, config::constants::theme::HOVER_TAB_BACKGROUND);

    std::cout << "[Font]" << std::endl;
    printVal(state->font.color, config::constants::font::COLOR);
    printVal(state->font.family, config::constants::font::FAMILY);
    printVal(state->font.ligatures, config::constants::font::LIGATURES);
    printVal(state->font.size, config::constants::font::SIZE);
    printVal(state->font.line_height, config::constants::font::LINE_HEIGHT);

    std::cout << "[UI]" << std::endl;
    printVal(state->ui.cursor_style, config::constants::ui::CURSOR_STYLE);
    printVal(state->ui.line_numbers, config::constants::ui::LINE_NUMBERS);
    printVal(state->ui.status_bar_visible, config::constants::ui::STATUS_BAR_VISIBLE);
    printVal(state->ui.tab_bar_visible, config::constants::ui::TAB_BAR_VISIBLE);
    printVal(state->ui.highlight_current_line, config::constants::ui::HIGHLIGHT_CURRENT_LINE);
    printVal(state->ui.show_whitespace, config::constants::ui::SHOW_WHITESPACE);
    printVal(state->ui.show_indent_guides, config::constants::ui::SHOW_INDENT_GUIDES);
    printVal(state->ui.ui_scale, config::constants::ui::UI_SCALE);

    std::cout << "[Preference]" << std::endl;
    printVal(state->preference.tab_width, config::constants::preference::TAB_WIDTH);
    printVal(state->preference.auto_format, config::constants::preference::AUTO_FORMAT);
    printVal(state->preference.bracket_matching, config::constants::preference::BRACKET_MATCHING);
    printVal(state->preference.auto_close_brackets, config::constants::preference::AUTO_CLOSE_BRACKETS);
    printVal(state->preference.word_wrap, config::constants::preference::WORD_WRAP);
    printVal(state->preference.trim_trailing_whitespace_on_save, config::constants::preference::TRIM_TRAILING_WHITESPACE_ON_SAVE);
    printVal(state->preference.highlight_active_scope, config::constants::preference::HIGHLIGHT_ACTIVE_SCOPE);
    printVal(state->preference.auto_indent, config::constants::preference::AUTO_INDENT);

    std::cout << "[Input]" << std::endl;
    printVal(state->input.shortcut_save, config::constants::input::SHORTCUT_SAVE);
    printVal(state->input.shortcut_search, config::constants::input::SHORTCUT_SEARCH);
    printVal(state->input.shortcut_split_horizontal, config::constants::input::SHORTCUT_SPLIT_HORIZONTAL);
    printVal(state->input.shortcut_split_vertical, config::constants::input::SHORTCUT_SPLIT_VERTICAL);
    printVal(state->input.vim_mode, config::constants::input::VIM_MODE);
    printVal(state->input.autocomplete, config::constants::input::AUTOCOMPLETE);
    printVal(state->input.clipboard_integration, config::constants::input::CLIPBOARD_INTEGRATION);
    printVal(state->input.mouse_selection, config::constants::input::MOUSE_SELECTION);
    printVal(state->input.drag_and_drop, config::constants::input::DRAG_AND_DROP);

    std::cout << "[Plugins]" << std::endl;
    printVal(state->plugins.lsp, config::constants::plugins::LSP);
    printVal(state->plugins.snippets, config::constants::plugins::SNIPPETS);
    printVal(state->plugins.git, config::constants::plugins::GIT);
    printVal(state->plugins.linter, config::constants::plugins::LINTER);
    printVal(state->plugins.file_explorer, config::constants::plugins::FILE_EXPLORER);
    printVal(state->plugins.hot_reload, config::constants::plugins::HOT_RELOAD);

    std::cout << "[File]" << std::endl;
    printVal(state->file.autosave_mode, config::constants::file::AUTOSAVE_MODE);
    printVal(state->file.exclude_patterns, config::constants::file::EXCLUDE_PATTERNS);
    printVal(state->file.show_hidden_files, config::constants::file::SHOW_HIDDEN_FILES);

    std::cout << "\n\n";
}

void eventLoop(SDL_Renderer *renderer, SDL_Window *window, config::ConfigWatcher *watcher, config::EditorConfig *state) {
    auto running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            }
        }

        watcher->check();
        drawBackground(renderer);
        drawCentreRect(renderer, window, state);
        SDL_RenderPresent(renderer);
    }
}

int main() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cerr << "SDL Init Error" << std::endl;
        exit(EXIT_FAILURE);
    }
    SDL_Window *window =
        SDL_CreateWindow("Blip", 0, 0, 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS);
    if (window == NULL) {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        std::cerr << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    config::EditorConfig state;
    config::ConfigWatcher watcher;

    // TODO: Calculate filepath using system file lookup
    std::string filepath = "config.ini";
    config::loadConfig(filepath, &state);
    printState(&state);
    watcher.start(filepath, [&state, filepath]() {
        config::loadConfig(filepath, &state);
        printState(&state);
    });

    eventLoop(renderer, window, &watcher, &state);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
