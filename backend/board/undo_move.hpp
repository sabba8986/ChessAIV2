#ifndef UNDO_MOVE
#define UNDO_MOVE

#include "piece.hpp"
#include <cstdint>

class UndoMove{
    int m_en_passant_sq;
    PieceType m_captured_piece_type;
    std::uint8_t m_castle_rights;
    int m_clock;

public:
    UndoMove(int enemy_en_passant_sq, PieceType captured_piece_type, std::uint8_t castle_rights, int clock);
    UndoMove(UndoMove&& other);
    UndoMove& operator=(UndoMove&& other);

    int en_passant_sq();
    PieceType captured_piece_type();
    std::uint8_t castle_rights();
    int clock();
};

#endif