#include "Board.hpp"

Board::Board(){
    white = defaults::white_init;
    black = defaults::black_init;
    pieces = defaults::pieces_init;
    all_white = defaults::all_white_init;
    all_black = defaults::all_black_init;
}

const std::array<Piece, 64>& Board::layout() const{
    return this->pieces;
}

const std::uint64_t Board::get_attack(int sq) const{
    const Piece& piece = pieces[sq];
    if(pieces[sq].is_white){
        return get_attack<true>(piece.type, sq);
    }
    return get_attack<false>(piece.type, sq);
}

