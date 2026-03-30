#pragma once
#include <blip/buffer/table.hpp>
#include <string>
#include <utility>
#include <vector>

namespace buffer {

typedef struct {
    PieceTable::State table_state;
    size_t cursor_position;
} EditRecord;

class EditorBuffer {
  public:
    explicit EditorBuffer(const std::string &initial_text = "");

    void insertText(const std::string &text);
    void backspace(size_t amount = 1);

    void commit();
    void undo();
    void redo();

    std::string getText() const;
    size_t getCursor() const;
    size_t getTotalLength() const;
    void setCursor(size_t new_pos);
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    std::pair<size_t, size_t> getCursorPosition2D() const;

  private:
    PieceTable table;
    size_t cursor_pos;
    std::vector<size_t> line_starts;
    void updateLineStarts(const std::string &text);

    std::vector<EditRecord> undo_stack;
    std::vector<EditRecord> redo_stack;
};
}
