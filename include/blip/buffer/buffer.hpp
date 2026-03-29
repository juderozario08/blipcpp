#pragma once
#include <blip/buffer/table.hpp>
#include <string>
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

  private:
    PieceTable table;
    size_t cursor_pos;

    std::vector<EditRecord> undo_stack;
    std::vector<EditRecord> redo_stack;
};
}
