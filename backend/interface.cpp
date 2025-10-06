#include "interface.hpp"
#include <memory>
#include <string>

Board *current_board = nullptr;

void create_board(){
    delete current_board;
    current_board = new Board();
}

const char *get_board_state(){
    return current_board->board_as_string();
}

