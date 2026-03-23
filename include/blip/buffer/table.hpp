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
    std::string getText() const;

  private:
    std::string original_buffer;
    std::string add_buffer;
    std::vector<Piece> pieces;
};
}
