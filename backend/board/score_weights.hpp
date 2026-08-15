#ifndef SCORE_WEIGHTS
#define SCORE_WEIGHTS
#include "piece.hpp"

namespace material_weights{
    constexpr int pawn_weight = 1;
    constexpr int bishop_weight = 3;
    constexpr int knight_weight = 4;
    constexpr int rook_weight = 5;
    constexpr int queen_weight = 9;
}

namespace mobility_weights{
    constexpr int bishop_weight = 3;
    constexpr int knight_weight = 2;
    constexpr int rook_weight = 4;
    constexpr int queen_weight = 6;
}


constexpr int get_weight(PieceType p){
    using enum PieceType;
    using namespace material_weights;
    switch(p){
        case PAWN: return pawn_weight;
        case ROOK: return rook_weight;
        case KNIGHT: return knight_weight;
        case BISHOP: return bishop_weight;
        case QUEEN: return queen_weight;
        default: return 0;
    }
}



#endif