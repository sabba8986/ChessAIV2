#include "Board.hpp"
#include "constants.hpp"
#include "attacks.hpp"


Board::Board(){
    reset();
}

const std::array<Piece, 64>& Board::layout() const{
    return this->pieces;
}

std::uint64_t Board::get_attack(int sq) const{
    using namespace attacks;
    const Piece piece = pieces[sq];
    std::uint64_t allies = all_pieces[piece.color];
    std::uint64_t enemies = all_pieces[!piece.color];
    switch(piece.type){
        case ROOK: return rook_attacks(sq, allies, enemies);
        case BISHOP: return bishop_attacks(sq, allies, enemies);
        case QUEEN: return bishop_attacks(sq, allies, enemies) | rook_attacks(sq, allies, enemies);
        case KNIGHT: return knight_attacks(sq, allies, enemies);
        case KING: return king_attacks(sq, allies, enemies);
        case PAWN: return pawn_attacks(sq, piece.color, allies, enemies);
        default: return 0;
    }
}

void Board::make_move(int s, int d){
    Piece& src = pieces[s];
    Piece& dest = pieces[d];
    std::uint64_t dest_mask = 1ull << d;
    std::uint64_t move_mask = dest_mask | (1ull << s);
    bitboards[src.color][src.type] ^= move_mask;
    all_pieces[src.color] ^= move_mask;
    bitboards[!src.color][dest.type] &= ~dest_mask;
    all_pieces[!src.color] &= ~dest_mask;
    dest = src;
    src.type = EMPTY;

}

std::uint64_t Board::checkers(Color c){
    using namespace attacks;
    int king = std::countr_zero(bitboards[c][KING]);
    auto& attackers = bitboards[c];
    std::uint64_t allies = all_pieces[c];
    std::uint64_t enemies = all_pieces[!c];
    return 
        (knight_attacks(king, allies, enemies) & attackers[KNIGHT]) | 
        (bishop_attacks(king, allies, enemies) & (attackers[BISHOP] | attackers[QUEEN])) | 
        (rook_attacks(king, allies, enemies) & (attackers[ROOK] | attackers[QUEEN])) | 
        (pawn_captures(king, c, enemies) & attackers[PAWN]);
}

bool Board::in_check(Color c){
    return checkers(c) != 0;
}

//assumes nonempty piece is at sq
std::uint64_t Board::pinner(int sq){
    const Piece piece = pieces[sq];
    std::uint64_t pos = 1ull << sq;
    std::uint64_t before_checkers = checkers(piece.color);
    bitboards[piece.color][piece.type] ^= pos;
    all_pieces[piece.color] ^= pos;
    std::uint64_t after_checkers = checkers(piece.color);
    bitboards[piece.color][piece.type] ^= pos;
    all_pieces[piece.color] ^= pos;
    return after_checkers ^ before_checkers; 
}

void Board::reset(){
    using namespace defaults;
    bitboards = {white_init, black_init};
    all_pieces = {all_white_init, all_black_init};
    pieces = pieces_init;
}


