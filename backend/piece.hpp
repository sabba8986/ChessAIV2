#ifndef PIECE
#define PIECE

enum PieceType{
    PAWN = 0, 
    ROOK = 1, 
    KNIGHT = 2, 
    BISHOP = 3, 
    QUEEN = 4, 
    KING = 5, 
    EMPTY = 6
};

enum Color{
    WHITE = 0, 
    BLACK = 1
};

struct Piece{
#ifdef __cplusplus
    Color color;
    PieceType type;
#else
    enum Color color;
    enum PieceType type;
#endif
};

#endif
