#ifndef BOARD_STATE
#define BOARD_STATE

#include "piece.hpp"
#include "move_list.hpp"
#ifdef __cplusplus
#include "board.hpp"
#include <array>
#endif


struct BoardState{
#ifdef __cplusplus
private:
    Color turn;
    std::array<bool, 2> in_check;
    std::array<MoveList, 64> moves;
    std::array<Piece, 64> pieces;

public: 
    BoardState(Board& board);
#else
    enum Color turn;
    bool in_check[2];
    struct MoveList moves[64];
    struct Piece pieces[64];
#endif
};


#endif