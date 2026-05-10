#include "constants.hpp"
#include "Board.hpp"

#include <string>
#include <iostream>

Board::Board(){
    white[PAWN_INDEX] = INIT_WHITE_PAWNS;
    white[ROOK_INDEX] = INIT_WHITE_ROOKS;
    white[KNIGHT_INDEX] = INIT_WHITE_KNIGHTS;
    white[BISHOP_INDEX] = INIT_WHITE_BISHOPS;
    white[QUEEN_INDEX] = INIT_WHITE_QUEEN;
    white[KING_INDEX] = INIT_WHITE_KING;

    black[PAWN_INDEX] = INIT_BLACK_PAWNS;
    black[ROOK_INDEX] = INIT_BLACK_ROOKS;
    black[KNIGHT_INDEX] = INIT_BLACK_KNIGHTS;
    black[BISHOP_INDEX] = INIT_BLACK_BISHOPS;
    black[QUEEN_INDEX] = INIT_BLACK_QUEEN;
    black[KING_INDEX] = INIT_BLACK_KING;
}

Piece *Board::layout(){
    return this->pieces.data();
}

