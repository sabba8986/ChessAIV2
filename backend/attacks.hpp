#ifndef ATTACKS
#define ATTACKS
#include "constants.hpp"
#include <cstdint>


namespace attacks{
    template<bool is_white>
    std::uint64_t pawn_attacks(int sq, std::uint64_t allies, std::uint64_t enemies){
        std::uint64_t pos{1u << sq};
        std::uint64_t occupancy = allies | enemies;
        if constexpr(is_white){
            std::uint64_t attack = bitboard::shift(pos, bitboard::N) & ~occupancy;
            if(attack && (pos & defaults::white_init[1])){
                attack |= bitboard::shift(attack, bitboard::N) & ~occupancy;
            }
            return (attack | bitboard::shift(pos, bitboard::NE) | bitboard::shift(pos, bitboard::NW)) & enemies;
        }
        else{
            std::uint64_t attack = bitboard::shift(pos, bitboard::S) & ~occupancy;
            if(attack && (pos & defaults::black_init[1])){
                attack |= bitboard::shift(attack, bitboard::S) & ~occupancy;
            }
            return (attack | bitboard::shift(pos, bitboard::SE) | bitboard::shift(pos, bitboard::SW)) & enemies;

        }
    }

    std::uint64_t rook_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t knight_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t bishop_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t queen_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t king_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
}

#endif
