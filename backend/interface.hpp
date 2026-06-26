#ifndef INTERFACE 
#define INTERFACE
#include "Piece.hpp"
#include <cstdint>


extern "C"{
    void create_board();
    const Piece *pieces();
    std::uint64_t get_legal_attacks(int sq);
    void make_move(int s, int d);
    bool in_check(Color c);
}

#endif
