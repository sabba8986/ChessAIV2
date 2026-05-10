#ifndef Board
#define BOARD

#include "constants.hpp"
#include "Piece.hpp"
#include <cstdint>
#include <string>

class Board{
    //bitboards for each color. Pieces are in the order of: 
    //{Pawn, Rook, Knight, Bishop, Queen, King}
    std::uint64_t white[6];
    std::uint64_t black[6];

public:
    Board();
    
    const char *board_as_string();
};


#endif
