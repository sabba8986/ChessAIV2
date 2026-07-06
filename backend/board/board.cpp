#include "board.hpp"
#include "constants.hpp"
#include "move.hpp"
#include "bitboard_moves.hpp"
#include "undo_move.hpp"
#include <bit>
#include <utility>

Board::Board(){
    reset();
}


const Piece *Board::layout() const{
    return this->pieces.data();
}


int Board::get_king_pos(Color c) const{
    return std::countr_zero(bitboards[c][KING]);
}


void Board::do_castle(int src, int dest){
    Color castler_color = pieces[src].color;
    pieces[src].type = EMPTY;
    pieces[dest].type = EMPTY;
    if(src < dest){
        bitboards[castler_color][KING] <<= 2;
        bitboards[castler_color][ROOK] ^= ((1ull << dest) | (bitboards[castler_color][KING] >> 1));
        pieces[src + 2] = pieces[src];
        pieces[src + 1] = pieces[dest];
    }
    else{
        bitboards[castler_color][KING] >>= 2;
        bitboards[castler_color][ROOK] ^= ((1ull << dest) | (bitboards[castler_color][KING] << 1));
        pieces[src - 2] = pieces[src];
        pieces[src - 1] = pieces[dest];
    }
    castle_rights &= ~(castler_color == WHITE ? (white_left_castle_allowed_flag | white_right_castle_allowed_flag) : 
                                                     (black_left_castle_allowed_flag | black_right_castle_allowed_flag));
}


void Board::move_piece(int src, int dest){
    const Piece attacker = pieces[src];
    const Piece captured = pieces[dest];
    std::uint64_t dest_mask = 1ull << dest;
    std::uint64_t move_mask = (1ull << src) | dest;
    bitboards[attacker.color][attacker.type] ^= move_mask;
    pieces[src].type = EMPTY;
    if(captured.type != EMPTY){
        bitboards[captured.color][captured.type] ^= dest;
    }
    pieces[dest] = attacker;
    if(attacker.type == ROOK){
        if(src > get_king_pos(attacker.color)){
            castle_rights &= ~(attacker.color == WHITE ? white_left_castle_allowed_flag : black_left_castle_allowed_flag);
        }
        else{
            castle_rights &= ~(attacker.color == WHITE ? white_right_castle_allowed_flag : black_right_castle_allowed_flag);
        }
    }
    else if(attacker.type == KING){
        castle_rights &= ~(attacker.color == WHITE ? (white_left_castle_allowed_flag | white_right_castle_allowed_flag) : 
                                                     (black_left_castle_allowed_flag | black_right_castle_allowed_flag));
    }
}


void Board::promote(int sq, PieceType p){
    Color c = pieces[sq].color;
    std::uint64_t pos = 1ull << sq;
    bitboards[c][PAWN] ^= pos;
    bitboards[c][p] ^= pos;
    pieces[sq].type = p;
}


void Board::do_en_passant(int en_passant_sq){
    Color c = pieces[en_passant_sq].color;
    int captured_piece_sq = c == WHITE ? en_passant_sq - 8 : en_passant_sq + 8;
    bitboards[c][PAWN] ^= (1ull << captured_piece_sq);
    pieces[captured_piece_sq].type = EMPTY;
}


void Board::make_move(Move move){
    int src = move.src();
    int dest = move.dest();
    prev_moves.emplace(move, UndoMove(en_passant_sq, pieces[dest].type, castle_rights, clock));
    if(move.is_castle()){
        do_castle(src, dest);
    }
    else{
        move_piece(src, dest);
        if(move.is_promotion()){
            promote(src, move.promoted_type());
        }
        else if(move.is_en_passant()){
            do_en_passant(src);
        }
    }
    en_passant_sq = 0;
    turn = static_cast<Color>(!turn);
    recalculate_all_pieces();
}


void Board::recalculate_all_pieces(){
    all_pieces[WHITE] = bitboards[WHITE][KING] | bitboards[WHITE][QUEEN] | bitboards[WHITE][PAWN] | 
        bitboards[WHITE][BISHOP] | bitboards[WHITE][KNIGHT] | bitboards[WHITE][ROOK];
    all_pieces[BLACK] = bitboards[BLACK][KING] | bitboards[BLACK][QUEEN] | bitboards[BLACK][PAWN] | 
        bitboards[BLACK][BISHOP] | bitboards[BLACK][KNIGHT] | bitboards[BLACK][ROOK];
}


std::uint64_t Board::get_attackers(int sq, Color attacker_color) const{
    auto& attackers = bitboards[attacker_color];
    std::uint64_t allies = all_pieces[!attacker_color] & ~(1ull << sq);
    std::uint64_t enemies = all_pieces[attacker_color];
    return 
        (bitboard_moves::knight(sq, allies, enemies) & attackers[KNIGHT]) | 
        (bitboard_moves::bishop(sq, allies, enemies) & (attackers[BISHOP] | attackers[QUEEN])) | 
        (bitboard_moves::rook(sq, allies, enemies) & (attackers[ROOK] | attackers[QUEEN])) | 
        (bitboard_moves::pawn_captures(sq, static_cast<Color>(!attacker_color), enemies) & attackers[PAWN]) |
        (bitboard_moves::king(sq, allies, enemies) & attackers[KING]);
}


void Board::undo_castle(int src, int dest){
    Color castler_color = pieces[src].color;
    if(src < dest){
        pieces[src] = pieces[src + 2];
        pieces[dest] = pieces[src + 1];
        bitboards[castler_color][KING] >>= 2;
        bitboards[castler_color][ROOK] ^= ((1ull << dest) | (bitboards[castler_color][KING] >> 1));
        pieces[src + 2].type = EMPTY;
        pieces[src + 1].type = EMPTY;
    }
    else{
        pieces[src] = pieces[src - 2];
        pieces[dest] = pieces[src - 1];
        bitboards[castler_color][KING] <<= 2;
        bitboards[castler_color][ROOK] ^= ((1ull << dest) | (bitboards[castler_color][KING] << 1));
        pieces[src - 2].type = EMPTY;
        pieces[src - 1].type = EMPTY;
    }
}


void Board::undo_move_piece(int src, int dest, PieceType captured_piece_type){
    const Piece attacker = pieces[dest];
    std::uint64_t dest_mask = 1ull << dest;
    std::uint64_t move_mask = (1ull << src) | dest;
    bitboards[attacker.color][attacker.type] ^= move_mask;
    pieces[src] = pieces[dest];
    pieces[dest].type = EMPTY;
    if(captured_piece_type != EMPTY){
        bitboards[!attacker.color][captured_piece_type] |= dest;
        pieces[dest].type = captured_piece_type;
    }
}


void Board::undo_promote(int sq, PieceType p){
    Color c = pieces[sq].color;
    std::uint64_t pos = 1ull << sq;
    bitboards[c][PAWN] ^= pos;
    bitboards[c][p] ^= pos;
    pieces[sq].type = PAWN;
}


void Board::undo_en_passant(int en_passant_sq){
    Color c = pieces[en_passant_sq].color;
    int captured_piece_sq = c == WHITE ? en_passant_sq - 8 : en_passant_sq + 8;
    bitboards[c][PAWN] ^= (1ull << captured_piece_sq);
    pieces[captured_piece_sq].type = PAWN;
}


void Board::undo_last_move(){
    auto [move, undo_info] = std::move(prev_moves.top());
    prev_moves.pop();
    int src = move.src();
    int dest = move.dest();
    if(move.is_castle()){
        undo_castle(src, dest);
    }
    else{
        if(move.is_promotion()){
            undo_promote(dest, move.promoted_type());
        }
        undo_move_piece(src, dest, undo_info.captured_piece_type());
        if(move.is_en_passant()){
            undo_en_passant(undo_info.en_passant_sq());
        }
    }
    en_passant_sq = undo_info.en_passant_sq();
    castle_rights = undo_info.castle_rights();
    turn = static_cast<Color>(!turn);
    clock = undo_info.clock();
}


std::uint64_t Board::get_checkers(Color c) const{
    return get_attackers(get_king_pos(c), static_cast<Color>(!c));
}


//assumes nonempty piece is at sq
std::uint64_t Board::pinner(int sq){
    const Piece piece = pieces[sq];
    std::uint64_t pos = 1ull << sq;
    std::uint64_t prev_checkers = get_checkers(piece.color);
    bitboards[piece.color][piece.type] ^= pos;
    all_pieces[piece.color] ^= pos;
    std::uint64_t after_checkers = get_checkers(piece.color);
    bitboards[piece.color][piece.type] ^= pos;
    all_pieces[piece.color] ^= pos;
    return prev_checkers ^ after_checkers; 
}


std::uint64_t Board::get_quiets_and_captures(int sq) const{
    const Piece piece = pieces[sq];
    std::uint64_t allies = all_pieces[piece.color];
    std::uint64_t enemies = all_pieces[!piece.color];
    switch(piece.type){
        case ROOK: return bitboard_moves::rook(sq, allies, enemies);
        case BISHOP: return bitboard_moves::bishop(sq, allies, enemies);
        case QUEEN: return bitboard_moves::queen(sq, allies, enemies);
        case KNIGHT: return bitboard_moves::knight(sq, allies, enemies);
        case KING: return bitboard_moves::king(sq, allies, enemies);
        case PAWN: return bitboard_moves::pawn(sq, piece.color, allies, enemies);
        default: return 0;
    }
}


std::uint64_t Board::get_promotion_row(Color c){
    return c == WHITE ? 255ull << 56: 255ull;
}


//An exception will occur at get_checkers if the user tries to move a piece that is already attacking the opponent king. 
//This is because when testing all possible moves that piece may make for legality, 
//one of those moves will involve capturing the opponent king. 
//It is not possible to determine if the opponent king is in check, if it doesn't exist on the board!
//With turns-based moves, this exception will never occur
std::uint64_t Board::get_legal_quiets_and_captures(int sq){
    const Piece piece = pieces[sq];
    //cannot block a double check with a non-king piece
    if(piece.type == EMPTY || (std::popcount(get_checkers(piece.color)) == 2 && piece.type != KING)){
        return 0;
    }
    std::uint64_t attacks = get_quiets_and_captures(sq);
    if(piece.type == KING){
        std::uint64_t legal_moves = 0;
        bitboards[piece.color][KING] = 0;
        while(attacks){
            int dest = std::countr_zero(attacks);
            std::uint64_t dest_mask = 1ull << dest;
            if(get_attackers(dest, static_cast<Color>(!piece.color)) == 0){
                legal_moves |= dest_mask;
            }
            attacks ^= dest_mask;
        }
        bitboards[piece.color][KING] = 1ull << sq;
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
            int checker_sq = std::countr_zero(get_checkers(piece.color));
            int king_sq = std::countr_zero(bitboards[piece.color][KING]);
            std::uint64_t line_of_attack = tables::attack_from_piece_to_king[checker_sq][king_sq];
            return line_of_attack & attacks;
        }
        else{
            return attacks;
        }
    }
}


std::uint64_t Board::get_castle_moves(Color c){
    std::uint64_t moves = 0;
    Color attacker_color = static_cast<Color>(!c);
    if(get_checkers(c) != 0) return moves;
    if(c == WHITE){
        if((castle_rights & white_left_castle_allowed_flag) && get_attackers(4, attacker_color) && get_attackers(5, attacker_color)){
            moves |= (1ull << 7);
        }
        if((castle_rights & white_left_castle_allowed_flag) && get_attackers(2, attacker_color) && get_attackers(1, attacker_color)){
            moves |= 1;
        }   
    }
    else{
        if((castle_rights & black_left_castle_allowed_flag) && get_attackers(60, attacker_color) && get_attackers(61, attacker_color)){
            moves |= (1ull << 63);
        }
        if((castle_rights & black_right_castle_allowed_flag) && get_attackers(57, attacker_color) && get_attackers(58, attacker_color)){
            moves |= (1ull << 56);
        }
    }
    return moves;
}


BoardState Board::get_board_state(){
    return BoardState(*this);
}


MoveList Board::get_legal_moves(int sq){
    MoveList legal_moves;
    const Piece piece = pieces[sq];
    std::uint64_t quiets_and_captures = get_legal_quiets_and_captures(sq);
    std::uint64_t enemies = all_pieces[!piece.color];
    std::uint64_t en_passant = piece.type == PAWN ? bitboard_moves::pawn_en_passant(sq, piece.color, en_passant_sq) : 0;
    std::uint64_t castles = piece.type == KING ? get_castle_moves(piece.color) : 0;
    while(quiets_and_captures){
        int dest = std::countr_zero(quiets_and_captures);
        std::uint64_t dest_mask = 1ull << dest;
        std::uint16_t flags = 0;
        if(dest_mask & enemies){
            flags |= Move::capture_flag;
        }
        if(dest_mask & get_promotion_row(piece.color)){
            flags |= Move::promotion_flag;
            legal_moves.add_move(Move(sq, dest, flags, QUEEN));
            legal_moves.add_move(Move(sq, dest, flags, KNIGHT));
            legal_moves.add_move(Move(sq, dest, flags, ROOK));
            legal_moves.add_move(Move(sq, dest, flags, BISHOP));
        } 
        else{
            legal_moves.add_move(Move(sq, dest, flags));
        }
    }
    if(en_passant){
        legal_moves.add_move(Move(sq, std::countr_zero(en_passant), Move::en_passant_flag));
    }
    while(castles){
        int dest = std::countr_zero(castles);
        legal_moves.add_move(Move(sq, dest, Move::castle_flag));
        castles ^= (1ull << dest);
    }
    return legal_moves;
}


bool Board::in_check(Color c){
    return get_checkers(c) != 0;
}


void Board::reset(){
    using namespace defaults;
    bitboards = {white_init, black_init};
    all_pieces = {all_white_init, all_black_init};
    pieces = pieces_init;
    en_passant_sq = 0;
    castle_rights = castle_rights_init;
    clock = 0;
    turn = WHITE;
}