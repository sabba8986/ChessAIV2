#include "interface.hpp"
#include "Board.hpp"
#include <cstdint>
#include <x86intrin.h>
#include <iostream>

Board current_board;

void create_board(){
    current_board.reset();
}

int type_of_piece(int i){
    return static_cast<int>((current_board.layout())[i].type);
}

bool is_white_piece(int i){
    return (current_board.layout())[i].is_white;
}

std::uint64_t get_attack(int sq){
    //unsigned long long start = __rdtsc();
    std::uint64_t c = current_board.get_attack(sq);
    //unsigned long long end = __rdtsc();
    //std::cout << "Attacks generated in " << end - start << " cycles" << std::endl;
    return c;
}

void make_move(int s, int d){
    current_board.make_move(s, d);
}
