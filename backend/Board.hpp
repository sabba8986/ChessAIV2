#ifndef Board
#define BOARD

#include "Piece.hpp"
#include <cstdint>
#include <array>

class Board{
    //bitboards for each color. Pieces are in the order of: 
    //{Pawn, Rook, Knight, Bishop, Queen, King}
    std::array<std::uint64_t, 6> white;
    std::array<std::uint64_t, 6> black;

    std::array<Piece, 64> pieces;
public:
    Board();
    Piece *layout();
};


#endif
