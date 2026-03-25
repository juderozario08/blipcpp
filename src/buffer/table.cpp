#include <blip/buffer/table.hpp>

namespace buffer {

PieceTable::PieceTable(const std::string &initial_text) : original_buffer(initial_text) {
    if (!initial_text.empty()) {
        pieces.push_back({BufType::ORIGINAL, 0, initial_text.length()});
    }
}

const std::string PieceTable::getText() {
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

void PieceTable::insert(size_t index, const std::string &text) {
    if (text.empty())
        return;

    size_t add_offset = add_buffer.length();
    add_buffer += text;

    size_t current_length = 0;

    for (size_t i = 0; i < pieces.size(); ++i) {
        if (current_length + pieces[i].length > index) {
            size_t offset_in_piece = index - current_length;

            if (offset_in_piece == 0) {
                if (i > 0 && pieces[i - 1].source == BufType::ADD && pieces[i - 1].start + pieces[i - 1].length == add_offset) {

                    pieces[i - 1].length += text.length();
                    return;
                }
                Piece new_piece = {BufType::ADD, add_offset, text.length()};
                pieces.insert(pieces.begin() + i, new_piece);
            } else {
                Piece original_piece = pieces[i];
                Piece left_half = {original_piece.source, original_piece.start, offset_in_piece};
                Piece right_half = {original_piece.source, original_piece.start + offset_in_piece,
                                    original_piece.length - offset_in_piece};

                pieces[i] = left_half;
                Piece new_piece = {BufType::ADD, add_offset, text.length()};
                pieces.insert(pieces.begin() + i + 1, new_piece);
                pieces.insert(pieces.begin() + i + 2, right_half);
            }
            return;
        }
        current_length += pieces[i].length;
    }

    if (!pieces.empty() && pieces.back().source == BufType::ADD && pieces.back().start + pieces.back().length == add_offset) {

        pieces.back().length += text.length();
    } else {
        Piece new_piece = {BufType::ADD, add_offset, text.length()};
        pieces.push_back(new_piece);
    }
}

void PieceTable::erase(size_t index, size_t length) {
    if (length == 0)
        return;

    size_t current_length = 0;
    size_t start_piece_idx = pieces.size();
    size_t start_offset = 0;

    for (size_t i = 0; i < pieces.size(); ++i) {
        if (current_length + pieces[i].length > index) {
            start_piece_idx = i;
            start_offset = index - current_length;
            break;
        }
        current_length += pieces[i].length;
    }

    if (start_piece_idx == pieces.size())
        return;

    size_t chars_to_delete = length;
    size_t i = start_piece_idx;

    while (chars_to_delete > 0 && i < pieces.size()) {
        Piece &p = pieces[i];
        size_t available = p.length - start_offset;

        if (chars_to_delete >= available) {
            if (start_offset == 0) {
                pieces.erase(pieces.begin() + i);
            } else {
                p.length = start_offset;
                i++;
            }
            chars_to_delete -= available;
            start_offset = 0;
        } else {
            if (start_offset == 0) {
                p.start += chars_to_delete;
                p.length -= chars_to_delete;
            } else {
                Piece right_half = {p.source, p.start + start_offset + chars_to_delete,
                                    p.length - start_offset - chars_to_delete};
                p.length = start_offset;
                pieces.insert(pieces.begin() + i + 1, right_half);
            }
            chars_to_delete = 0;
        }
    }
}

const size_t PieceTable::length() {
    size_t total = 0;
    for (const auto &piece : pieces) {
        total += piece.length;
    }
    return total;
}

const size_t PieceTable::getPieceCount() { return pieces.size(); }
}
