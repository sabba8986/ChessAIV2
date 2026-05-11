#include "interface.hpp"
#include "Board.hpp"

Board *current_board = nullptr;

void create_board(){
    delete current_board;
    current_board = new Board();
}

int type_of_piece(int i){
    return static_cast<int>((current_board->layout())[i].type);
}

bool is_white_piece(int i){
    return (current_board->layout())[i].is_white;
}
