#ifndef ATTACKS
#define ATTACKS
#include "constants.hpp"
#include <cstdint>
#include <x86intrin.h>
#include <iostream>

namespace attacks{
    template<bool is_white>
    std::uint64_t pawn_attacks(int sq, std::uint64_t allies, std::uint64_t enemies){
        using namespace bitboard;
        constexpr std::uint64_t white_two_square_mask = slide<N>(defaults::white_init[static_cast<int>(PieceType::PAWN)]);
        constexpr std::uint64_t black_two_square_mask = slide<S>(defaults::black_init[static_cast<int>(PieceType::PAWN)]);
        std::uint64_t pos = 1ull << sq;
        std::uint64_t occupancy = allies | enemies;
        if constexpr(is_white){
            std::uint64_t forward = slide<N>(pos) & ~occupancy;
            forward |= slide<N>(forward & white_two_square_mask);
            std::uint64_t captures = (slide<NE>(pos) | slide<NW>(pos)) & enemies; 
            return forward | captures;
        }
        else{
            std::uint64_t forward = slide<S>(pos) & ~occupancy;
            forward |= slide<S>(forward & black_two_square_mask);
            std::uint64_t captures = (slide<SE>(pos) | slide<SW>(pos)) & enemies;
            return forward | captures;
        }
    }

    std::uint64_t rook_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t knight_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t bishop_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t queen_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t king_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
}

#endif
