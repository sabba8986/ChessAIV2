#ifndef PIECE
#define PIECE

typedef enum{
    PAWN = 0, 
    ROOK = 1, 
    KNIGHT = 2, 
    BISHOP = 3, 
    QUEEN = 4, 
    KING = 5, 
    EMPTY = 6
} PieceType;

typedef enum{
    WHITE = 0, 
    BLACK = 1
} Color;

typedef struct{
    Color color;
    PieceType type;
} Piece;

#endif
