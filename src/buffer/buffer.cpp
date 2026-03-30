#include <blip/buffer/buffer.hpp>
#include <utility>

namespace buffer {

EditorBuffer::EditorBuffer(const std::string &initial_text) : table(initial_text), cursor_pos(0) {
    updateLineStarts(initial_text);
    commit();
}

void EditorBuffer::updateLineStarts(const std::string &text) {
    for (int i = 0; i < text.length(); i++) {
        if (text[i] == '\n') {
            line_starts.push_back(cursor_pos + i + 1);
        }
    }
}

void EditorBuffer::commit() {
    if (!redo_stack.empty()) {
        redo_stack.clear();
    }
    undo_stack.push_back(EditRecord{table.getState(), cursor_pos});
}

void EditorBuffer::undo() {
    if (undo_stack.empty())
        return;
    redo_stack.push_back(EditRecord{table.getState(), cursor_pos});
    EditRecord record = std::move(undo_stack.back());
    table.restoreState(record.table_state);
    setCursor(record.cursor_position);
    undo_stack.pop_back();
}

void EditorBuffer::redo() {
    if (redo_stack.empty())
        return;
    undo_stack.push_back(EditRecord{table.getState(), cursor_pos});
    EditRecord record = std::move(redo_stack.back());
    table.restoreState(record.table_state);
    setCursor(record.cursor_position);
    redo_stack.pop_back();
}

std::string EditorBuffer::getText() const { return table.getText(); }

size_t EditorBuffer::getCursor() const { return cursor_pos; }

size_t EditorBuffer::getTotalLength() const { return table.getTotalLength(); }

void EditorBuffer::setCursor(size_t new_pos) {
    if (new_pos > table.getTotalLength()) {
        cursor_pos = table.getTotalLength();
    } else if (new_pos < 0) {
        cursor_pos = 0;
    } else {
        cursor_pos = new_pos;
    }
}

void EditorBuffer::insertText(const std::string &text) {
    if (text.empty()) {
        return;
    }
    updateLineStarts(text);
    table.insert(cursor_pos, text);
    cursor_pos += text.length();
}

void EditorBuffer::backspace(size_t amount) {
    if (amount == 0 || cursor_pos == 0) {
        return;
    }
    if (cursor_pos < amount) {
        amount = cursor_pos;
    }
    table.erase(cursor_pos, amount);
    setCursor(cursor_pos - amount);
}

void EditorBuffer::moveLeft() { setCursor(cursor_pos - 1); }
void EditorBuffer::moveRight() { setCursor(cursor_pos + 1); }
void EditorBuffer::moveUp() {}
void EditorBuffer::moveDown() {}

std::pair<size_t, size_t> EditorBuffer::getCursorPosition2D() const { return {0, 0}; }
}
