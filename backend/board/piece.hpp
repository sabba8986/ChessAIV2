#ifndef PIECE
#define PIECE

#include <cassert>
#include <cstdint>


enum class PieceType: std::uint8_t{
    EMPTY = 0, 
    PAWN = 1,
    ROOK = 2, 
    KNIGHT = 3, 
    BISHOP = 4, 
    QUEEN = 5, 
    KING = 6,
};

enum class Color: std::uint8_t{
    WHITE = 0, 
    BLACK = 1
};

enum class Piece: std::uint8_t{
    EMPTY = 0,
    WHITE_PAWN = 1, 
    WHITE_ROOK = 2,
    WHITE_KNIGHT = 3,
    WHITE_BISHOP = 4,
    WHITE_QUEEN = 5,
    WHITE_KING = 6,
    BLACK_PAWN = 9,
    BLACK_ROOK = 10,
    BLACK_KNIGHT = 11,
    BLACK_BISHOP = 12,
    BLACK_QUEEN = 13,
    BLACK_KING = 14
};


constexpr Piece to_piece(Color c, PieceType p){
    assert(p != PieceType::EMPTY && "Cannot convert empty PieceType to empty Piece using color. Please use Piece::EMPTY directly");
    return static_cast<Piece>(static_cast<std::uint8_t>(p) | (static_cast<std::uint8_t>(c) << 3));
}
constexpr Color get_color(Piece p){
    assert(p != Piece::EMPTY && "Cannot get the color of an empty piece");
    return static_cast<Color>((static_cast<std::uint8_t>(p) & 0b1000) >> 3);
}
constexpr PieceType get_type(Piece p){
    return static_cast<PieceType>(static_cast<int>(p) & 0b0111);
}
constexpr std::size_t to_idx(Piece p){
    return static_cast<std::size_t>(p);
}
constexpr Piece to_piece(int p){
    return static_cast<Piece>(p);
}
constexpr std::size_t to_idx(Color c){
    return static_cast<std::size_t>(c);
}
constexpr Color to_color(int c){
    return static_cast<Color>(c);
}
constexpr std::size_t to_idx(PieceType p){
    return static_cast<std::size_t>(p);
}
constexpr PieceType to_type(int p){
    return static_cast<PieceType>(p);
}
constexpr Color other_color(Color c){
    return c == Color::WHITE ? Color::BLACK : Color::WHITE;
}
template<Color c>
constexpr Color other_color(){
    return c == Color::WHITE ? Color::BLACK : Color::WHITE;
}
#endif
