#ifndef PIECE
#define PIECE

enum class PieceType: int{
    PAWN = 0, 
    ROOK = 1, 
    KNIGHT = 2, 
    BISHOP = 3, 
    QUEEN = 4, 
    KING = 5, 
    EMPTY = 6
};

struct Piece{
    bool is_white;
    PieceType type;
};

#endif
