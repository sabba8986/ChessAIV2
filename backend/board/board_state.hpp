#ifndef BOARD_STATE
#define BOARD_STATE

#include "piece.hpp"
#include "move_list.hpp"
#include <array>

class Board;


struct BoardState{
private:
    std::array<bool, 2> m_in_check;
    std::array<MoveList, 64> m_move_lists;
    std::array<Piece, 64> m_pieces;

public: 
    BoardState(Board& board);
    bool in_check(Color c);
    MoveList move_list(int sq);
    Piece piece(int sq);
};


#endif