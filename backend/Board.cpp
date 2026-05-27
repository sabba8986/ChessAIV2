#include "Board.hpp"

Board::Board(){
    reset();
}

const std::array<Piece, 64>& Board::layout() const{
    return this->pieces;
}

std::uint64_t Board::get_attack(int sq) const{
    const Piece piece = pieces[sq];
    if(piece.color == WHITE){
        return do_get_attack<WHITE>(piece.type, sq);
    }
    return do_get_attack<BLACK>(piece.type, sq);
}

void Board::make_move(int s, int d){
    if(pieces[s].color == WHITE){
        do_make_move<WHITE>(s, d);
    }
    else{
        do_make_move<BLACK>(s, d);
    }
}

std::uint64_t Board::checkers(Color c){
    return c == WHITE ? checkers<WHITE>() : checkers<BLACK>();
}

void Board::remove_bitboard_piece(int sq){
    std::uint64_t pos = 1ull << sq;
    const Piece piece = pieces[sq];
    if(piece.color == WHITE){
        white[piece.type] ^= pos;
        all_white ^= pos;
    }
    else{
        black[piece.type] ^= pos;
        all_black ^= pos;
    }
}

void Board::add_bitboard_piece(int sq, Piece piece){
    std::uint64_t pos = 1ull << sq;;
    if(piece.color == WHITE){
        white[piece.type] |= pos;
        all_white |= pos;
    }
    else{
        black[piece.type] |= pos;
        all_black |= pos;
    }
}

//assumes nonempty piece is at sq
std::uint64_t Board::pinner(int sq){
    Color ally = pieces[sq].color;
    std::uint64_t before_checkers = checkers(ally);
    remove_bitboard_piece(sq);
    std::uint64_t after_checkers = checkers(ally);
    add_bitboard_piece(sq, pieces[sq]);
    return after_checkers ^ before_checkers; 
}

void Board::reset(){
    white = defaults::white_init;
    black = defaults::black_init;
    pieces = defaults::pieces_init;
    all_white = defaults::all_white_init;
    all_black = defaults::all_black_init;
}


