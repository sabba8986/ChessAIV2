#ifndef BOARD
#define BOARD

#include "Piece.hpp"
#include "attacks.hpp"
#include <cstdint>
#include <array>
#include <iostream>

class Board{
    //bitboards for each color. Pieces are in the order of: 
    //{Pawn, Rook, Knight, Bishop, Queen, King}
    std::array<std::uint64_t, 6> white;
    std::array<std::uint64_t, 6> black;
    std::uint64_t all_white;
    std::uint64_t all_black;
    std::array<Piece, 64> pieces;

    template<bool is_white>
    const std::uint64_t get_attack(PieceType p, int sq) const{
        std::uint64_t allies = is_white ? all_white : all_black;
        std::uint64_t enemies = is_white ? all_black : all_white;
        switch(p){
            case PieceType::ROOK: return attacks::rook_attacks(sq, allies, enemies);
            case PieceType::BISHOP: return attacks::bishop_attacks(sq, allies, enemies);
            case PieceType::QUEEN: return attacks::bishop_attacks(sq, allies, enemies) | attacks::rook_attacks(sq, allies, enemies);
            case PieceType::KNIGHT: return attacks::knight_attacks(sq, allies, enemies);
            case PieceType::KING: return attacks::king_attacks(sq, allies, enemies);
            case PieceType::PAWN: return attacks::pawn_attacks<is_white>(sq, allies, enemies);
            default: return 0;
        }
    }

public:
    Board();
    const std::array<Piece, 64>& layout() const;

    const std::uint64_t get_attack(int sq) const;
};


#endif
