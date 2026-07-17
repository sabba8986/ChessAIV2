#ifndef MOVES
#define MOVES
#include "piece.hpp"
#include <cstdint>
#include <x86intrin.h>


namespace bitboard_moves{
    std::uint64_t pawn_captures(int sq, Color c, std::uint64_t enemies);
    std::uint64_t pawn_en_passant(int sq, Color c, int en_passant_sq);
    std::uint64_t pawn(int sq, Color c, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t rook(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t knight(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t bishop(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t queen(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t king(int sq, std::uint64_t allies, std::uint64_t enemies);
}
#endif
