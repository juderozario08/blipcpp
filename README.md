# Blip Text Editor

A high-performance text editor built from scratch, focusing on low-level systems programming, Vim emulation, and efficient cross-platform file watching.

## 🛠 Tech Stack & APIs

This project utilizes the following libraries and OS-specific APIs:

* **Window Management:** [SDL2](https://www.libsdl.org/)
* **Font Rendering:** [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf)
* **File Watching (macOS):** [kqueue](https://man.freebsd.org/cgi/man.cgi?query=kqueue)
* **File Watching (Linux):** [inotify](https://man7.org/linux/man-pages/man7/inotify.7.html)

---

## 📖 Table of Contents

1. [Configuration Rules](#configuration-rules)
2. [Theme Configuration](#theme-configuration)
3. [Font Configuration](#font-configuration)
4. [UI Configuration](#ui-configuration)
5. [Preferences Configuration](#preferences-configuration)
6. [Input Configuration](#input-configurations)
7. [Plugins](#plugins)
8. [File Configuration](#file-configuration)

---

## Configuration Rules

* **Booleans:** Must be `true` or `false`.
* **Colors:** Formatted in **RGBA Hex** (e.g., `#RRGGBBAA`).
* **Shortcuts:** Must have a Modifier key and keys must be separated by the `+` symbol (e.g., `Control+s`).
* **Case Sensitivity:** Keybinds are case sensitive.

---

## Theme Configuration

```sh
background = #000000
foreground = #D4D4D4
cursor = #FFFFFF
selection = #2F2F2F
line_number = #858585
whitespace = #3B3B3B
diff_add = #587C0C
diff_remove = #94151B
diff_change = #005A9E
diagnostic_info = #75BEFF
diagnostic_error = #F48771
diagnostic_warning = #CCA700
popup_background = #252526
tooltip_border = #454545
completion_background = #2D2D30
hover_tab_background = #2A2D2E
```

## Font Configuration

```sh
font_family = JetBrains Mono
font_color = #D4D4D4
font_ligatures = true
font_size = 14
line_height = 1.5
```

## UI Configuration

```sh
cursor_style = 0 # 0 = Block cursor, 1 = Line cursor
line_numbers = 0 # 0 = Absolute, 1 = Relative, 2 = Hidden, 3 = AbsoluteAndRelative
status_bar_visible = true
tab_bar_visible = true
highlight_current_line = true
show_whitespace = false
show_indent_guides = true
ui_scale = 100 # Percent based
```

## Preferences Configuration

```sh
tab_width = 4
auto_format = 0 # 0 = Manual, 1 = OnSave, 2 = OnPaste
bracket_matching = true
auto_close_brackets = true
word_wrap = false
trim_trailing_whitespace_on_save = true
highlight_active_scope = true
auto_indent = true
```

## Input Configuration

```sh
shortcut_save = Control+s
shortcut_search = Control+f
shortcut_split_vertical = Control+V
shortcut_split_horizontal = Control+H
vim_mode = true
autocomplete = true
clipboard_integration = true
mouse_selection = true
drag_and_drop = false
```

## Plugins

```sh
lsp = false
snippets = false
git = false
linter = false
file_explorer = false
hot_reload = true
```

## File Configuration

```sh
autosave_mode = 0 # 0 = SaveOnFocus, 1 = SaveDelay, 2 = SaveManual
exclude_patterns = [".git", "node_modules", "dist"]
show_hidden_files = false
```
