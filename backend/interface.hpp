#ifndef INTERFACE 
#define INTERFACE 


#include "Board.hpp"
#include <string>


extern "C"{
    void create_board();

    const char *get_board_state();
}

#endif
