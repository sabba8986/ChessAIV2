#ifndef MOVE_INFO
#define MOVE_INFO

#include "Piece.hpp"
#include <cstdint>
#include <array>

enum MoveType {
    NON_CAPTURE = 0, 
    CAPTURE = 1, 
    CASTLE = 2, 
    EN_PASSANT = 3, 
    PROMOTION = 4
};

typedef struct {
    int from; 
    int to;
    Piece attacker;
    PieceType captured_piece_type;
    std::array<std::uint64_t, 2> prev_checker;
} MoveInfo;

#endif
