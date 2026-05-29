#include "interface.hpp"
#include "Board.hpp"
#include <cstdint>
#include <chrono>
#include <iostream>


Board current_board;

void create_board(){
    current_board.reset();
}

int type_of_piece(int i){
    return static_cast<int>((current_board.layout())[i].type);
}

bool is_white_piece(int i){
    return (current_board.layout())[i].color == WHITE;
}

std::uint64_t get_legal_attacks(int sq){
    auto attacks = current_board.get_legal_attacks(sq);
    return attacks;
}

void make_move(int s, int d){
    current_board.make_move(s, d);
}
