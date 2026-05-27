#ifndef PIECE
#define PIECE

enum PieceType: int{
    PAWN = 0, 
    ROOK = 1, 
    KNIGHT = 2, 
    BISHOP = 3, 
    QUEEN = 4, 
    KING = 5, 
    EMPTY = 6
};

enum Color: int{
    WHITE = 0, 
    BLACK = 1
};


struct Piece{
    Color color;
    PieceType type;
};

#endif
