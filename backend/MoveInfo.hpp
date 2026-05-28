#ifndef MOVE_INFO
#define MOVE_INFO

#include "Piece.hpp"

struct MoveInfo{
    int from; 
    int to;
    Piece attacker;
    PieceType captured_piece_type;
    bool prev_check_status;
};

#endif
