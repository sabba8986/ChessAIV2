#include "interface.hpp"
#include "Board.hpp"

Board *current_board = nullptr;

void create_board(){
    delete current_board;
    current_board = new Board();
}

Piece *get_board_state(){
    return current_board->layout();
}

