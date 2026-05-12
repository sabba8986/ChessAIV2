#include "constants.hpp"
#include "Board.hpp"


Board::Board(){
    white = defaults::white_init;
    black = defaults::black_init;
    pieces = defaults::pieces_init;
}

const std::array<Piece, 64>& Board::layout() const{
    return this->pieces;
}

