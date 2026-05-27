#ifndef ATTACKS
#define ATTACKS
#include "constants.hpp"
#include <cstdint>
#include <x86intrin.h>


namespace attacks{
    std::uint64_t pawn_captures(int sq, Color c, std::uint64_t enemies);
    std::uint64_t pawn_attacks(int sq, Color c, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t rook_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t knight_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t bishop_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t queen_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t king_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
}

#endif
