#include <charconv>
#include <config/editor_config.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

inline constexpr const int OPACITY = 255;

namespace config {
std::string trim(const std::string &s) {
    size_t first = s.find_first_not_of(" \t\r\n");
    if (std::string::npos == first)
        return "";
    size_t last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, (last - first + 1));
}

std::pair<std::string, std::string> parseLine(const std::string &line) {
    if (line.empty() || line[0] == '#' || line[0] == ';') {
        return {"", ""};
    }

    size_t delimiterPos = line.find('=');
    if (delimiterPos != std::string::npos) {
        std::string key = trim(line.substr(0, delimiterPos));
        std::string value = trim(line.substr(delimiterPos + 1));
        return {key, value};
    }

    return {"", ""};
}

std::optional<Color> hexToColor(std::string_view hex) {
    if (hex.empty() || hex[0] != '#' || (hex.length() != 7 && hex.length() != 9)) {
        return std::nullopt;
    }

    config::Color color;
    auto parse = [&](std::string_view sub, Uint8 &out) -> bool {
        auto [ptr, ec] = std::from_chars(sub.data(), sub.data() + sub.size(), out, 16);
        return ec == std::errc{};
    };

    if (!parse(hex.substr(1, 2), color.r))
        return std::nullopt;
    if (!parse(hex.substr(3, 2), color.g))
        return std::nullopt;
    if (!parse(hex.substr(5, 2), color.b))
        return std::nullopt;

    if (hex.length() == 9) {
        if (!parse(hex.substr(7, 2), color.a))
            return std::nullopt;
    } else {
        color.a = 255;
    }

    return color;
}

void handleColorUpdate(std::string value, Color *color, Color def) {
    if (auto c = hexToColor(value)) {
        *color = *c;
    } else {
        *color = def;
    }
}

void handleBoolUpdate(std::string value, bool *dst, bool def) {
    if (value == "true") {
        *dst = true;
    } else if (value == "false") {
        *dst = false;
    } else {
        *dst = def;
    }
}

// TODO: COMPLETE THE LOGIC IN THIS FUNCTION TO CHECK SHORTCUT KEYBIND SYNTAX
void handleShortcutString(std::string value, std::string *dst, const char *def) {
    if (value.length()) {
        *dst = def;
        return;
    }
    *dst = value;
}

void handleThemeConfigUpdates(std::string key, std::string value, EditorConfig *state) {
    if (key == constants::theme::BACKGROUND) {
        handleColorUpdate(value, &state->theme.background, defaults::theme::BACKGROUND);
    }
    if (key == constants::theme::FOREGROUND) {
        handleColorUpdate(value, &state->theme.foreground, defaults::theme::FOREGROUND);
    }
    if (key == constants::theme::CURSOR) {
        handleColorUpdate(value, &state->theme.cursor, defaults::theme::CURSOR);
    }
    if (key == constants::theme::SELECTION) {
        handleColorUpdate(value, &state->theme.selection, defaults::theme::SELECTION);
    }
    if (key == constants::theme::LINE_NUMBER) {
        handleColorUpdate(value, &state->theme.line_number, defaults::theme::LINE_NUMBER);
    }
    if (key == constants::theme::WHITESPACE) {
        handleColorUpdate(value, &state->theme.whitespace, defaults::theme::WHITESPACE);
    }
    if (key == constants::theme::DIFF_ADD) {
        handleColorUpdate(value, &state->theme.diff_add, defaults::theme::DIFF_ADD);
    }
    if (key == constants::theme::DIFF_REMOVE) {
        handleColorUpdate(value, &state->theme.diff_remove, defaults::theme::DIFF_REMOVE);
    }
    if (key == constants::theme::DIFF_CHANGE) {
        handleColorUpdate(value, &state->theme.diff_change, defaults::theme::DIFF_CHANGE);
    }
    if (key == constants::theme::DIAGNOSTIC_INFO) {
        handleColorUpdate(value, &state->theme.diagnostic_info, defaults::theme::DIAGNOSTIC_INFO);
    }
    if (key == constants::theme::DIAGNOSTIC_ERROR) {
        handleColorUpdate(value, &state->theme.diagnostic_error, defaults::theme::DIAGNOSTIC_ERROR);
    }
    if (key == constants::theme::DIAGNOSTIC_WARNING) {
        handleColorUpdate(value, &state->theme.diagnostic_warning, defaults::theme::DIAGNOSTIC_WARNING);
    }
    if (key == constants::theme::POPUP_BACKGROUND) {
        handleColorUpdate(value, &state->theme.popup_background, defaults::theme::POPUP_BACKGROUND);
    }
    if (key == constants::theme::TOOLTIP_BORDER) {
        handleColorUpdate(value, &state->theme.tooltip_border, defaults::theme::TOOLTIP_BORDER);
    }
    if (key == constants::theme::COMPLETION_BACKGROUND) {
        handleColorUpdate(value, &state->theme.completion_background, defaults::theme::COMPLETION_BACKGROUND);
    }
    if (key == constants::theme::HOVER_TAB_BACKGROUND) {
        handleColorUpdate(value, &state->theme.hover_tab_background, defaults::theme::HOVER_TAB_BACKGROUND);
    }
}

void handleFontConfigUpdates(std::string key, std::string value, EditorConfig *state) {
    // Font Config Updates
    if (key == constants::font::FAMILY) {
        // MAKE SURE TO CHECK IF THE "VALUE = FONT" FIRST EXISTS OR NOT
        state->font.family = value.length() > 0 ? value : defaults::font::FAMILY;
    }
    if (key == constants::font::COLOR) {
        handleColorUpdate(value, &state->font.color, defaults::font::COLOR);
    }
    if (key == constants::font::LIGATURES) {
        handleBoolUpdate(value, &state->font.ligatures, defaults::font::LIGATURES);
    }
    if (key == constants::font::SIZE) {
        try {
            auto size = static_cast<int>(std::stoi(value));
            state->font.size = size <= 0 ? defaults::font::SIZE : size;
        } catch (...) {
            state->font.size = defaults::font::SIZE;
        }
    }
    if (key == constants::font::LINE_HEIGHT) {
        try {
            auto height = static_cast<float>(std::stof(value));
            state->font.line_height = height <= 0.0 ? defaults::font::LINE_HEIGHT : height;
        } catch (...) {
            state->font.line_height = defaults::font::LINE_HEIGHT;
        }
    }
}

void handleUIConfigUpdates(std::string key, std::string value, EditorConfig *state) {
    if (key == constants::ui::CURSOR_STYLE) {
        try {
            auto style = static_cast<CursorStyleOpts>(std::stoi(value));
            switch (style) {
            case CursorStyleOpts::CursorBlock:
            case CursorStyleOpts::CursorLine:
                state->ui.cursor_style = style;
                break;
            default:
                state->ui.cursor_style = defaults::ui::CURSOR_STYLE;
                break;
            }
        } catch (...) {
            state->ui.cursor_style = defaults::ui::CURSOR_STYLE;
        }
    }
    if (key == constants::ui::LINE_NUMBERS) {
        try {
            auto style = static_cast<LineNumberOpts>(std::stoi(value));
            switch (style) {
            case LineNumberOpts::LineAbsolute:
            case LineNumberOpts::LineHidden:
            case LineNumberOpts::LineRelative:
            case LineNumberOpts::LineAbsoluteAndRelative:
                state->ui.line_numbers = style;
                break;
            default:
                state->ui.line_numbers = defaults::ui::LINE_NUMBERS;
                break;
            }
        } catch (...) {
            state->ui.line_numbers = defaults::ui::LINE_NUMBERS;
        }
    }
    if (key == constants::ui::STATUS_BAR_VISIBLE) {
        handleBoolUpdate(value, &state->ui.status_bar_visible, defaults::ui::STATUS_BAR_VISIBLE);
    }
    if (key == constants::ui::TAB_BAR_VISIBLE) {
        handleBoolUpdate(value, &state->ui.tab_bar_visible, defaults::ui::TAB_BAR_VISIBLE);
    }
    if (key == constants::ui::HIGHLIGHT_CURRENT_LINE) {
        handleBoolUpdate(value, &state->ui.highlight_current_line, defaults::ui::HIGHLIGHT_CURRENT_LINE);
    }
    if (key == constants::ui::SHOW_WHITESPACE) {
        handleBoolUpdate(value, &state->ui.show_whitespace, defaults::ui::SHOW_WHITESPACE);
    }
    if (key == constants::ui::SHOW_INDENT_GUIDES) {
        handleBoolUpdate(value, &state->ui.show_indent_guides, defaults::ui::SHOW_INDENT_GUIDES);
    }
    if (key == constants::ui::UI_SCALE) {
        try {
            auto scale = static_cast<int>(std::stof(value));
            state->ui.ui_scale = scale <= 5 ? defaults::ui::UI_SCALE : scale;
        } catch (...) {
            state->ui.ui_scale = defaults::ui::UI_SCALE;
        }
    }
}

void handlePreferenceConfigUpdates(std::string key, std::string value, EditorConfig *state) {
    // Preference Config Update
    if (key == constants::preference::TAB_WIDTH) {
        try {
            auto width = static_cast<int>(std::stoi(value));
            state->preference.tab_width = width <= 2 ? defaults::preference::TAB_WIDTH : width;
        } catch (...) {
            state->preference.tab_width = defaults::preference::TAB_WIDTH;
        }
    }
    if (key == constants::preference::AUTO_FORMAT) {
        try {
            auto format = static_cast<AutoFormatOpts>(std::stoi(value));
            switch (format) {
            case AutoFormatOpts::FormatManual:
            case AutoFormatOpts::FormatOnSave:
            case AutoFormatOpts::FormatOnPaste:
                state->preference.auto_format = format;
                break;
            default:
                state->preference.auto_format = defaults::preference::AUTO_FORMAT;
                break;
            }
        } catch (...) {
            state->preference.auto_format = defaults::preference::AUTO_FORMAT;
        }
    }
    if (key == constants::preference::BRACKET_MATCHING) {
        handleBoolUpdate(value, &state->preference.bracket_matching, defaults::preference::BRACKET_MATCHING);
    }
    if (key == constants::preference::AUTO_CLOSE_BRACKETS) {
        handleBoolUpdate(value, &state->preference.auto_close_brackets, defaults::preference::AUTO_CLOSE_BRACKETS);
    }
    if (key == constants::preference::WORD_WRAP) {
        handleBoolUpdate(value, &state->preference.word_wrap, defaults::preference::WORD_WRAP);
    }
    if (key == constants::preference::TRIM_TRAILING_WHITESPACE_ON_SAVE) {
        handleBoolUpdate(value, &state->preference.trim_trailing_whitespace_on_save,
                         defaults::preference::TRIM_TRAILING_WHITESPACE_ON_SAVE);
    }
    if (key == constants::preference::HIGHLIGHT_ACTIVE_SCOPE) {
        handleBoolUpdate(value, &state->preference.highlight_active_scope, defaults::preference::HIGHLIGHT_ACTIVE_SCOPE);
    }
    if (key == constants::preference::AUTO_INDENT) {
        handleBoolUpdate(value, &state->preference.auto_indent, defaults::preference::AUTO_INDENT);
    }
}

void handleInputConfigUpdates(std::string key, std::string value, EditorConfig *state) {
    // Input Config Update
    if (key == constants::input::SHORTCUT_SAVE) {
        handleShortcutString(value, &state->input.shortcut_save, defaults::input::SHORTCUT_SAVE);
    }
    if (key == constants::input::SHORTCUT_SEARCH) {
        handleShortcutString(value, &state->input.shortcut_search, defaults::input::SHORTCUT_SEARCH);
    }
    if (key == constants::input::SHORTCUT_SPLIT_VERTICAL) {
        handleShortcutString(value, &state->input.shortcut_split_vertical, defaults::input::SHORTCUT_SPLIT_VERTICAL);
    }
    if (key == constants::input::SHORTCUT_SPLIT_HORIZONTAL) {
        handleShortcutString(value, &state->input.shortcut_split_horizontal, defaults::input::SHORTCUT_SPLIT_HORIZONTAL);
    }
    if (key == constants::input::VIM_MODE) {
        handleBoolUpdate(value, &state->input.vim_mode, defaults::input::VIM_MODE);
    }
    if (key == constants::input::AUTOCOMPLETE) {
        handleBoolUpdate(value, &state->input.autocomplete, defaults::input::AUTOCOMPLETE);
    }
    if (key == constants::input::CLIPBOARD_INTEGRATION) {
        handleBoolUpdate(value, &state->input.clipboard_integration, defaults::input::CLIPBOARD_INTEGRATION);
    }
    if (key == constants::input::MOUSE_SELECTION) {
        handleBoolUpdate(value, &state->input.mouse_selection, defaults::input::MOUSE_SELECTION);
    }
    if (key == constants::input::DRAG_AND_DROP) {
        handleBoolUpdate(value, &state->input.drag_and_drop, defaults::input::DRAG_AND_DROP);
    }
}

void handlePluginsConfigUpdates(std::string key, std::string value, EditorConfig *state) {
    // Plugins Config Updates
    if (key == constants::plugins::LSP) {
        handleBoolUpdate(value, &state->plugins.lsp, defaults::plugins::LSP);
    }
    if (key == constants::plugins::SNIPPETS) {
        handleBoolUpdate(value, &state->plugins.snippets, defaults::plugins::SNIPPETS);
    }
    if (key == constants::plugins::GIT) {
        handleBoolUpdate(value, &state->plugins.git, defaults::plugins::GIT);
    }
    if (key == constants::plugins::LINTER) {
        handleBoolUpdate(value, &state->plugins.linter, defaults::plugins::LINTER);
    }
    if (key == constants::plugins::FILE_EXPLORER) {
        handleBoolUpdate(value, &state->plugins.file_explorer, defaults::plugins::FILE_EXPLORER);
    }
    if (key == constants::plugins::HOT_RELOAD) {
        handleBoolUpdate(value, &state->plugins.hot_reload, defaults::plugins::HOT_RELOAD);
    }
}

void handleFileConfigUpdates(std::string key, std::string value, EditorConfig *state) {
    // File Config Updates
    if (key == constants::file::AUTOSAVE_MODE) {
        try {
            switch (auto mode = static_cast<AutoSaveModeOpts>(std::stoi(value))) {
            case AutoSaveModeOpts::SaveManual:
            case AutoSaveModeOpts::SaveDelay:
            case AutoSaveModeOpts::SaveOnFocus:
                state->file.autosave_mode = mode;
                break;
            default:
                state->file.autosave_mode = defaults::file::AUTOSAVE_MODE;
                break;
            }
        } catch (...) {
            state->file.autosave_mode = defaults::file::AUTOSAVE_MODE;
        }
    }
    if (key == constants::file::EXCLUDE_PATTERNS) {
        // TODO: Handle logic for converting string to vector
    }
    if (key == constants::file::AUTOSAVE_MODE) {
        handleBoolUpdate(value, &state->file.show_hidden_files, defaults::file::SHOW_HIDDEN_FILES);
    }
}

void updateState(std::string key, std::string value, EditorConfig *state) {
    if (key.empty() || value.empty()) {
        return;
    }
    handleThemeConfigUpdates(key, value, state);
    handleFontConfigUpdates(key, value, state);
    handleUIConfigUpdates(key, value, state);
    handlePreferenceConfigUpdates(key, value, state);
    handleInputConfigUpdates(key, value, state);
    handlePluginsConfigUpdates(key, value, state);
    handleFileConfigUpdates(key, value, state);
}

void loadConfig(std::string filepath, EditorConfig *state) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Could not open " << filepath << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        auto [key, value] = parseLine(line);
        updateState(key, value, state);
    }
}
}
