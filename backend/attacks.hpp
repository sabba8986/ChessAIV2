#ifndef ATTACKS
#define ATTACKS
#include "constants.hpp"
#include <cstdint>

namespace attacks{
    template<bool is_white>
    std::uint64_t pawn_attacks(int sq, std::uint64_t allies, std::uint64_t enemies){
        std::uint64_t pos = std::uint64_t{1} << sq;
        std::uint64_t occupancy = allies | enemies;
        if constexpr(is_white){
            std::uint64_t attack = bitboard::slide<bitboard::N>(pos) & ~occupancy;
            if(attack && (pos & defaults::white_init[static_cast<int>(PieceType::PAWN)])){
                attack |= bitboard::slide<bitboard::N>(attack) & ~occupancy;
            }
            return attack | ((bitboard::slide<bitboard::NE>(pos) | bitboard::slide<bitboard::NW>(pos)) & enemies);
        }
        else{
            std::uint64_t attack = bitboard::slide<bitboard::S>(pos) & ~occupancy;
            if(attack && (pos & defaults::black_init[static_cast<int>(PieceType::PAWN)])){
                attack |= bitboard::slide<bitboard::S>(attack) & ~occupancy;
            }
            return attack | ((bitboard::slide<bitboard::SE>(pos) | bitboard::slide<bitboard::SW>(pos)) & enemies);

        }
    }

    std::uint64_t rook_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t knight_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t bishop_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t queen_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
    std::uint64_t king_attacks(int sq, std::uint64_t allies, std::uint64_t enemies);
}

#endif
