#ifndef PIECE
#define PIECE

#include <cassert>
enum class PieceType{
    EMPTY = 0, 
    PAWN = 1,
    ROOK = 2, 
    KNIGHT = 3, 
    BISHOP = 4, 
    QUEEN = 5, 
    KING = 6,
};

enum class Color{
    WHITE = 0, 
    BLACK = 1
};


//Value 7 is skipped for less conditional conversion between Color/PieceType combinations and Piece enum values
enum class Piece{
    EMPTY = 0,
    WHITE_PAWN = 1, 
    WHITE_ROOK = 2,
    WHITE_KNIGHT = 3,
    WHITE_BISHOP = 4,
    WHITE_QUEEN = 5,
    WHITE_KING = 6,
    BLACK_PAWN = 8,
    BLACK_ROOK = 9,
    BLACK_KNIGHT = 10,
    BLACK_BISHOP = 11,
    BLACK_QUEEN = 12,
    BLACK_KING = 13
};


constexpr Piece to_piece(Color c, PieceType p){
    return p == PieceType::EMPTY ? Piece::EMPTY : static_cast<Piece>(7 * static_cast<int>(c) + static_cast<int>(p));
}
constexpr Color get_color(Piece p){
    assert(p != Piece::EMPTY && "Cannot get the color of an empty piece");
    return static_cast<Color>(static_cast<int>(p)/7);
}
constexpr PieceType get_type(Piece p){
    return static_cast<PieceType>(static_cast<int>(p) % 7);
}
constexpr int to_int(Piece p){
    return static_cast<int>(p);
}
constexpr Piece to_piece(int p){
    return static_cast<Piece>(p);
}
constexpr int to_int(Color c){
    return static_cast<int>(c);
}
constexpr Color to_color(int c){
    return static_cast<Color>(c);
}
constexpr int to_int(PieceType p){
    return static_cast<int>(p);
}
constexpr PieceType to_type(int p){
    return static_cast<PieceType>(p);
}
constexpr Color other_color(Color c){
    return c == Color::WHITE ? Color::BLACK : Color::WHITE;
}
#endif
