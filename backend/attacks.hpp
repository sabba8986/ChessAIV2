#ifndef ATTACKS
#define ATTACKS
#include "constants.hpp"
#include <cstdint>
#include <x86intrin.h>


namespace attacks{
    template<Color c>
    std::uint64_t pawn_captures(int sq, std::uint64_t enemies){
        using namespace bitboard;
        std::uint64_t pos = 1ull << sq;
        if constexpr(c == WHITE){
            return (slide<NE>(pos) | slide<NW>(pos)) & enemies;
        }
        else{
            return (slide<SE>(pos) | slide<SW>(pos)) & enemies;
        }
    }

    template<Color c>
    std::uint64_t pawn_attacks(int sq, std::uint64_t allies, std::uint64_t enemies){
        using namespace bitboard;
        constexpr std::uint64_t white_two_square_mask = slide<N>(defaults::white_init[PAWN]);
        constexpr std::uint64_t black_two_square_mask = slide<S>(defaults::black_init[PAWN]);
        std::uint64_t pos = 1ull << sq;
        std::uint64_t occupancy = allies | enemies;
        std::uint64_t forward;
        if constexpr(c == WHITE){
            forward = slide<N>(pos) & ~occupancy;
            forward |= slide<N>(forward & white_two_square_mask);
        }
        else{
            forward = slide<S>(pos) & ~occupancy;
            forward |= slide<S>(forward & black_two_square_mask);
        }
        return forward | pawn_captures<c>(sq, enemies);
    }

    std::uint64_t rook_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t knight_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t bishop_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t queen_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t king_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
}

#endif
