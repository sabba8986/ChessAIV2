#ifndef PIECE
#define PIECE

enum class PieceType: int{
    EMPTY = 0,
    PAWN = 1, 
    ROOK = 2, 
    KNIGHT = 3, 
    BISHOP = 4, 
    QUEEN = 5, 
    KING = 6
};

struct Piece{
    bool is_white;
    PieceType type;
};

#endif
