#ifndef BOARD
#define BOARD

#include "Piece.hpp"
#include "MoveInfo.hpp"
#include <cstdint>
#include <array>

class Board{
    //bitboards for each color ({WHITE, BLACK}). Pieces are in the order of: 
    //{Pawn, Rook, Knight, Bishop, Queen, King, Empty}
    std::array<std::array<std::uint64_t, 7>, 2> bitboards;
    std::array<std::uint64_t, 2> all_pieces;
    std::array<Piece, 64> pieces;
    std::array<std::uint64_t, 2> checkers;



    std::uint64_t get_checkers(Color c) const;
    std::uint64_t pinner(int sq);

    void undo_previous_move(MoveInfo move);

public:
    Board();
    const Piece *layout() const; //for C API
    std::uint64_t get_attacks(int sq) const;
    std::uint64_t get_legal_attacks(int sq);
    MoveInfo make_move(int s, int d);
    bool in_check(Color c);
    void reset();
};
#endif
