#ifndef BOARD
#define BOARD

#include "Piece.hpp"
#include <cstdint>
#include <array>


class Board{
    //bitboards for each color ({WHITE, BLACK}). Pieces are in the order of: 
    //{Pawn, Rook, Knight, Bishop, Queen, King, Empty}
    std::array<std::array<std::uint64_t, 7>, 2> bitboards;
    std::array<std::uint64_t, 2> all_pieces;
    std::array<Piece, 64> pieces;

    void do_make_move(int s, int d);
    std::uint64_t checkers(Color c);
    std::uint64_t pinner(int sq);

public:
    Board();
    const std::array<Piece, 64>& layout() const;
    std::uint64_t get_attack(int sq) const;
    void make_move(int s, int d);
    bool in_check(Color c);
    void reset();
};
#endif
