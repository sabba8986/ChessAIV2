#ifndef INTERFACE 
#define INTERFACE 
#include <stddef.h>

#include "Piece.hpp"


extern "C"{
    void create_board();
    int type_of_piece(int i);
    bool is_white_piece(int i);
}

#endif
