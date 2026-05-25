#include "Board.hpp"

Board::Board(){
    reset();
}

const std::array<Piece, 64>& Board::layout() const{
    return this->pieces;
}

const std::uint64_t Board::get_attack(int sq) const{
    const Piece piece = pieces[sq];
    if(piece.is_white){
        return get_attack<true>(piece.type, sq);
    }
    return get_attack<false>(piece.type, sq);
}

void Board::make_move(int s, int d){
    Piece& src = pieces[s];
    Piece& dest = pieces[d];
    std::uint64_t dest_mask = 1ull << d;
    std::uint64_t move_mask = dest_mask | (1ull << s);
    if(src.is_white){
        white[static_cast<int>(src.type)] ^= move_mask;
        all_white ^= move_mask;
        if(dest.type != PieceType::EMPTY){
            black[static_cast<int>(dest.type)] &= ~dest_mask;
            all_black &= ~dest_mask;
        }
    }
    else{
        black[static_cast<int>(src.type)] ^= move_mask;
        all_black ^= move_mask;
        if(dest.type != PieceType::EMPTY){
            white[static_cast<int>(dest.type)] &= ~dest_mask;
            all_white &= ~dest_mask;
        }
    }
    dest = src;
    src.type = PieceType::EMPTY;
}


void Board::reset(){
    white = defaults::white_init;
    black = defaults::black_init;
    pieces = defaults::pieces_init;
    all_white = defaults::all_white_init;
    all_black = defaults::all_black_init;
}
