#ifndef UNDO_MOVE
#define UNDO_MOVE

#include "piece.hpp"
#include <cstdint>

class UndoMove{
    int m_en_passant_sq;
    PieceType m_captured_piece_type;
    std::uint8_t m_castle_rights;
    int m_clock;
    std::uint64_t m_prev_enemy_pinned;
    std::uint64_t m_prev_checkers;

public:
    UndoMove(int enemy_en_passant_sq, PieceType captured_piece_type, std::uint8_t castle_rights, int clock, std::uint64_t prev_enemy_pinned, std::uint64_t prev_checkers);
    UndoMove(UndoMove&& other);
    UndoMove& operator=(UndoMove&& other);

    int en_passant_sq();
    PieceType captured_piece_type();
    std::uint8_t castle_rights();
    int clock();
    std::uint64_t prev_enemy_pinned();
    std::uint64_t prev_checkers();
};

#endif