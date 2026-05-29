#ifndef MOVE_INFO
#define MOVE_INFO

#include "Piece.hpp"
#include <cstdint>
#include <array>

struct MoveInfo{
    int from; 
    int to;
    Piece attacker;
    PieceType captured_piece_type;
    std::array<std::uint64_t, 2> prev_checker;
};

#endif
