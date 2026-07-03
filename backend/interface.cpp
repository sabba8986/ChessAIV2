#include "interface.hpp"
#include "board.hpp"
#include <cstdint>


Board current_board;

void create_board(){
    current_board.reset();
}

const Piece *pieces(){
    return current_board.layout();
}

std::uint64_t get_legal_attacks(int sq){
    auto attacks = current_board.get_legal_attacks(sq);
    return attacks;
}

void make_move(int s, int d){
    current_board.make_move(s, d);
}

bool in_check(Color c){
    return current_board.in_check(c);
}
