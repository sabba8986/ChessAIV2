#include "constants.hpp"
#include "Board.hpp"

#include <string>
#include <iostream>

Board::Board(){
    white = defaults::white_init;
    black = defaults::black_init;
}

Piece *Board::layout(){
    return this->pieces.data();
}

