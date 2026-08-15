#ifndef BITBOARD_MOVES
#define BITBOARD_MOVES
#include "piece.hpp"
#include <cstdint>

namespace bitboard_moves{
    template<Color c>
    std::uint64_t pawn_captures(int sq, std::uint64_t enemies);

    template<Color c>
    std::uint64_t pawn_en_passant(int sq, int en_passant_sq);

    template<Color c>
    std::uint64_t pawn(int sq, std::uint64_t allies, std::uint64_t enemies);

    std::uint64_t rook(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t knight(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t bishop(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t queen(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t king(int sq, std::uint64_t allies, std::uint64_t enemies);
}

#include "bitboard_moves.inl"
#endif
