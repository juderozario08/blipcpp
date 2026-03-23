#include <blip/buffer/table.hpp>

namespace buffer {

PieceTable::PieceTable(const std::string &initial_text) : original_buffer(initial_text) {
    if (!initial_text.empty()) {
        pieces.push_back({BufType::ORIGINAL, 0, initial_text.length()});
    }
}

std::string PieceTable::getText() const {
    std::string result;

    for (const auto &piece : pieces) {
        if (piece.source == BufType::ORIGINAL) {
            result += original_buffer.substr(piece.start, piece.length);
        } else {
            result += add_buffer.substr(piece.start, piece.length);
        }
    }

    return result;
}

}
