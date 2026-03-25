#pragma once
#include <string>
#include <vector>

namespace buffer {

enum class BufType { ORIGINAL, ADD };

typedef struct {
    BufType source;
    size_t start;
    size_t length;
} Piece;

class PieceTable {
  public:
    PieceTable(const std::string &initial_text = "");
    const std::string getText();
    void insert(size_t index, const std::string &text);
    void erase(size_t index, size_t length);
    const size_t length();
    const size_t getPieceCount();

  private:
    std::string original_buffer;
    std::string add_buffer;
    std::vector<Piece> pieces;
};
}
