#ifndef INTERFACE 
#define INTERFACE 
#include <cstddef>
#include <cstdint>


extern "C"{
    void create_board();
    int type_of_piece(int i);
    bool is_white_piece(int i);
    std::uint64_t get_attack(int sq);
    void make_move(int s, int d);
}

#endif
