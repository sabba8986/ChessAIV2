#ifndef BITBOARD_MOVES_INL
#define BITBOARD_MOVES_INL
#include <cstdint>
#include "bitboard.hpp"
#include "pch.hpp"
#include "bitboard_moves.hpp"


template<Color c>
std::uint64_t bitboard_moves::pawn_captures(int sq, std::uint64_t enemies){
    using namespace bitboard;
    std::uint64_t pos = 1ull << sq;
    return ((c == Color::WHITE) ? slide<NE>(pos) | slide<NW>(pos) : slide<SE>(pos) | slide<SW>(pos)) & enemies;
}


template<Color c>
std::uint64_t bitboard_moves::pawn_en_passant(int sq, int en_passant_sq){
    return pawn_captures<c>(sq, 1ull << en_passant_sq);
}


template<Color c>
std::uint64_t bitboard_moves::pawn(int sq, std::uint64_t allies, std::uint64_t enemies){
    using namespace bitboard;
    constexpr std::uint64_t white_two_square_mask = slide<N>(tables::defaults::bitboards_init[static_cast<int>(Piece::WHITE_PAWN)]);
    constexpr std::uint64_t black_two_square_mask = slide<S>(tables::defaults::bitboards_init[static_cast<int>(Piece::BLACK_PAWN)]);
    std::uint64_t pos = 1ull << sq;
    std::uint64_t occupancy = allies | enemies;
    std::uint64_t forward;
    if constexpr(c == Color::WHITE){
        forward = slide<N>(pos) & ~occupancy;
        forward |= slide<N>(forward & white_two_square_mask) & ~occupancy;
    }
    else{
        forward = slide<S>(pos) & ~occupancy;
        forward |= slide<S>(forward & black_two_square_mask) & ~occupancy;
    }
    return forward | pawn_captures<c>(sq, enemies);
}

#endif