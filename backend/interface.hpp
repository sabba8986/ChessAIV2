#ifndef INTERFACE 
#define INTERFACE 


#include "Piece.hpp"


extern "C"{
    void create_board();

    Piece *get_board_state();
}

#endif
