#include "Board.hpp"
#include "constants.hpp"
#include "attacks.hpp"
#include <bit>


Board::Board(){
    reset();
}


const std::array<Piece, 64>& Board::layout() const{
    return this->pieces;
}


MoveInfo Board::make_move(int s, int d){
    Piece src = pieces[s];
    Piece dest = pieces[d];
    MoveInfo move = {
        s, 
        d, 
        src,
        dest.type,
        checkers
    };
    std::uint64_t dest_mask = 1ull << d;
    std::uint64_t move_mask = dest_mask | (1ull << s);
    bitboards[src.color][src.type] ^= move_mask;
    all_pieces[src.color] ^= move_mask;
    if(dest.type != EMPTY){
        bitboards[!src.color][dest.type] &= ~dest_mask;
        all_pieces[!src.color] &= ~dest_mask;
    }
    pieces[d] = src;
    pieces[s].type = EMPTY;

    checkers[src.color] = get_checkers(src.color);
    checkers[!src.color] = get_checkers(static_cast<Color>(!src.color));
    return move;
}


void Board::undo_previous_move(MoveInfo move){
    auto [s, d, attacker, captured_piece_type, prev_checkers] = move;
    pieces[s] = attacker;
    pieces[d].type = captured_piece_type;
    pieces[d].color = static_cast<Color>(!attacker.color);
    checkers = prev_checkers;
    std::uint64_t dest_mask = 1ull << d;
    std::uint64_t move_mask = dest_mask | (1ull << s);
    if(captured_piece_type != EMPTY){
        bitboards[!attacker.color][captured_piece_type] |= dest_mask;
        all_pieces[!attacker.color] |= dest_mask;
    }
    bitboards[attacker.color][attacker.type] ^= move_mask;
    all_pieces[attacker.color] ^= move_mask;
}


std::uint64_t Board::get_checkers(Color c) const{
    using namespace attacks;
    int king = std::countr_zero(bitboards[c][KING]);
    auto& attackers = bitboards[!c];
    std::uint64_t allies = all_pieces[c] & ~bitboards[c][KING];
    std::uint64_t enemies = all_pieces[!c];
    return 
        (knight_attacks(king, allies, enemies) & attackers[KNIGHT]) | 
        (bishop_attacks(king, allies, enemies) & (attackers[BISHOP] | attackers[QUEEN])) | 
        (rook_attacks(king, allies, enemies) & (attackers[ROOK] | attackers[QUEEN])) | 
        (pawn_captures(king, c, enemies) & attackers[PAWN]) |
        (king_attacks(king, allies, enemies) & attackers[KING]);
}


//assumes nonempty piece is at sq
std::uint64_t Board::pinner(int sq){
    const Piece piece = pieces[sq];
    std::uint64_t pos = 1ull << sq;
    bitboards[piece.color][piece.type] ^= pos;
    all_pieces[piece.color] ^= pos;
    std::uint64_t after_checkers = get_checkers(piece.color);
    bitboards[piece.color][piece.type] ^= pos;
    all_pieces[piece.color] ^= pos;
    return checkers[piece.color] ^ after_checkers; 
}


std::uint64_t Board::get_attacks(int sq) const{
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


//An exception will occur at get_checkers if the user tries to move a piece that is already attacking the opponent king. 
//This is because when testing all possible moves that piece may make for legality, 
//one of those moves will involve capturing the opponent king. 
//It is not possible to determine if the opponent king is in check, if it doesn't exist on the board!
//With turns-based moves, this exception will never occur
std::uint64_t Board::get_legal_attacks(int sq){
    const Piece piece = pieces[sq];
    //cannot block a double check with a non-king piece
    if(piece.type == EMPTY || (std::popcount(checkers[piece.color]) == 2 && piece.type != KING)){
        return 0;
    }
    std::uint64_t attacks = get_attacks(sq);
    if(piece.type == KING){
        std::uint64_t legal_moves = 0;
        while(attacks){
            std::uint64_t dest = std::countr_zero(attacks);
            std::uint64_t dest_mask = 1ull << dest;
            MoveInfo move = make_move(sq, dest);
            if(!in_check(piece.color)){
                legal_moves |= dest_mask;
            }
            undo_previous_move(move);
            attacks ^= dest_mask;
        }
        return legal_moves;
    }
    else{
        //if this point reached, piece is not king and there is at most one checker
        std::uint64_t pin = pinner(sq);
        if(pin != 0 && in_check(piece.color)){
            return 0;
        }
        else if(pin != 0){
            int pin_sq = std::countr_zero(pin);
            int king_sq = std::countr_zero(bitboards[piece.color][KING]);
            std::uint64_t line_of_attack = tables::attack_from_piece_to_king[pin_sq][king_sq];
            return line_of_attack & attacks;
        }
        else if(in_check(piece.color)){
            int checker_sq = std::countr_zero(checkers[piece.color]);
            int king_sq = std::countr_zero(bitboards[piece.color][KING]);
            std::uint64_t line_of_attack = tables::attack_from_piece_to_king[checker_sq][king_sq];
            return line_of_attack & attacks;
        }
        else{
            return attacks;
        }
    }
}


bool Board::in_check(Color c){
    return checkers[c] != 0;
}


void Board::reset(){
    using namespace defaults;
    bitboards = {white_init, black_init};
    all_pieces = {all_white_init, all_black_init};
    pieces = pieces_init;
    checkers = {0, 0};
}


