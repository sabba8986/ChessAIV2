#ifndef BOARD
#define BOARD

#include "Piece.hpp"
#include "attacks.hpp"
#include <cstdint>
#include <array>
#include <bit>

class Board{
    //bitboards for each color. Pieces are in the order of: 
    //{Pawn, Rook, Knight, Bishop, Queen, King, Empty}
    std::array<std::uint64_t, 7> white;
    std::array<std::uint64_t, 7> black;
    std::uint64_t all_white;
    std::uint64_t all_black;
    std::array<Piece, 64> pieces;

    template<Color c>
    std::uint64_t do_get_attack(PieceType p, int sq) const;

    template<Color c>
    void do_make_move(int s, int d);

    void remove_bitboard_piece(int sq);
    void add_bitboard_piece(int sq, Piece piece);


    std::uint64_t pinner(int sq);

    template<Color c>
    std::uint64_t checkers();

public:
    Board();
    const std::array<Piece, 64>& layout() const;

    std::uint64_t get_attack(int sq) const;

    void make_move(int s, int d);

    std::uint64_t checkers(Color c);

    template<Color c>
    bool in_check();

    void reset();

};


template<Color c>
std::uint64_t Board::do_get_attack(PieceType p, int sq) const{
    std::uint64_t allies = (c == WHITE) ? all_white : all_black;
    std::uint64_t enemies = (c == WHITE) ? all_black : all_white;
    switch(p){
        case ROOK: return attacks::rook_attacks(sq, allies, enemies);
        case BISHOP: return attacks::bishop_attacks(sq, allies, enemies);
        case QUEEN: return attacks::bishop_attacks(sq, allies, enemies) | attacks::rook_attacks(sq, allies, enemies);
        case KNIGHT: return attacks::knight_attacks(sq, allies, enemies);
        case KING: return attacks::king_attacks(sq, allies, enemies);
        case PAWN: return attacks::pawn_attacks<c>(sq, allies, enemies);
        default: return 0;
    }
}

template<Color c>
void Board::do_make_move(int s, int d){
    Piece& src = pieces[s];
    Piece& dest = pieces[d];
    std::uint64_t dest_mask = 1ull << d;
    std::uint64_t move_mask = dest_mask | (1ull << s);
    if constexpr(c == WHITE){
        white[src.type] ^= move_mask;
        all_white ^= move_mask;
        black[dest.type] &= ~dest_mask;
        all_black &= ~dest_mask;
    }
    else{
        black[src.type] ^= move_mask;
        all_black ^= move_mask;
        white[dest.type] &= ~dest_mask;
        all_white &= ~dest_mask;
    }
    dest = src;
    src.type = EMPTY;
}

template<Color c>
std::uint64_t Board::checkers(){
    using namespace attacks;
    int king = std::countr_zero((c == WHITE) ? white[KING] : black[KING]);
    auto& attackers = (c == WHITE) ? black : white;
    std::uint64_t allies = (c == WHITE) ? all_white : all_black;
    std::uint64_t enemies = (c == WHITE) ? all_black : all_white;
    return 
        (knight_attacks(king, allies, enemies) & attackers[KNIGHT]) | 
        (bishop_attacks(king, allies, enemies) & (attackers[BISHOP] | attackers[QUEEN])) | 
        (rook_attacks(king, allies, enemies) & (attackers[ROOK] | attackers[QUEEN])) | 
        (pawn_captures<c>(king, enemies) & attackers[PAWN]);
}

template<Color c>
bool Board::in_check(){
    return checkers<c>() != 0;
}


#endif
