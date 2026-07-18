#include "board.hpp"
#include "constants.hpp"
#include "move.hpp"
#include "bitboard_moves.hpp"
#include "undo_move.hpp"
#include <bit>
#include <utility>
#include <cassert>

Board::Board(){
    reset();
}


int Board::get_king_pos(Color c) const{
    return c == Color::WHITE ? std::countr_zero(bitboards[to_int(Piece::WHITE_KING)]) : 
                               std::countr_zero(bitboards[to_int(Piece::BLACK_KING)]);
}


void Board::do_castle(int src, int dest){
    Color castler_color = get_color(pieces[src]);
    int rook_idx = to_int(to_piece(castler_color, PieceType::ROOK));
    int king_idx = to_int(to_piece(castler_color, PieceType::KING));
    if(src < dest){
        bitboards[king_idx] <<= 2;
        bitboards[rook_idx] ^= ((1ull << dest) | (1ull << (src + 1)));
        pieces[src + 2] = pieces[src];
        pieces[src + 1] = pieces[dest];
    }
    else{
        bitboards[king_idx] >>= 2;
        bitboards[rook_idx] ^= ((1ull << dest) | (1ull << (src - 1)));
        pieces[src - 2] = pieces[src];
        pieces[src - 1] = pieces[dest];
    }
    pieces[src] = Piece::EMPTY;
    pieces[dest] = Piece::EMPTY;
    castle_rights &= ~(castler_color == Color::WHITE ? (white_left_castle_allowed_flag | white_right_castle_allowed_flag) : 
                                                     (black_left_castle_allowed_flag | black_right_castle_allowed_flag));
}


void Board::move_piece(int src, int dest){
    const Piece attacker = pieces[src];
    const Piece captured = pieces[dest];
    std::uint64_t dest_mask = 1ull << dest;
    std::uint64_t move_mask = (1ull << src) | dest_mask;
    bitboards[to_int(attacker)] ^= move_mask;
    pieces[src] = Piece::EMPTY;
    if(captured != Piece::EMPTY){
        bitboards[to_int(captured)] ^= dest_mask;
    }
    pieces[dest] = attacker;
    Color attacker_color = get_color(attacker);
    PieceType attacker_type = get_type(attacker);
    if(get_type(attacker) == PieceType::ROOK){
        if(src > get_king_pos(attacker_color)){
            castle_rights &= ~(attacker_color == Color::WHITE ? white_left_castle_allowed_flag : black_left_castle_allowed_flag);
        }
        else{
            castle_rights &= ~(attacker_color == Color::WHITE ? white_right_castle_allowed_flag : black_right_castle_allowed_flag);
        }
    }
    else if(get_type(attacker) == PieceType::KING){
        castle_rights &= ~(attacker_color == Color::WHITE ? (white_left_castle_allowed_flag | white_right_castle_allowed_flag) : 
                                                     (black_left_castle_allowed_flag | black_right_castle_allowed_flag));
    }
}


void Board::promote(int sq, PieceType p){
    std::uint64_t pos = 1ull << sq;
    Color c = get_color(pieces[sq]);
    Piece promoted_piece = to_piece(c, p);
    int pawn_idx = to_int(pieces[sq]);
    int promoted_idx = to_int(promoted_piece);
    bitboards[pawn_idx] ^= pos;
    bitboards[promoted_idx] ^= pos;
    pieces[sq] = promoted_piece;
}


void Board::do_en_passant(int en_passant_sq){
    Color enemy_color = other_color(turn);
    int pawn_idx = to_int(to_piece(enemy_color, PieceType::PAWN));
    int captured_piece_sq = turn == Color::WHITE ? en_passant_sq - 8 : en_passant_sq + 8;
    bitboards[pawn_idx] ^= (1ull << captured_piece_sq);
    pieces[captured_piece_sq] = Piece::EMPTY;
}


void Board::make_move(Move move){
    int src = move.src();
    int dest = move.dest();
    Piece moved = pieces[src];
    prev_moves.emplace(move, UndoMove(en_passant_sq, get_type(pieces[dest]), castle_rights, clock));
    if(move.is_castle()){
        do_castle(src, dest);
    }
    else{
        move_piece(src, dest);
        if(move.is_promotion()){
            promote(src, move.promoted_type());
        }
        else if(move.is_en_passant()){
            do_en_passant(en_passant_sq);
        }
    }
    if(moved == Piece::WHITE_PAWN && (dest - src == 16)){
        en_passant_sq = src + 8;
    }
    else if(moved == Piece::BLACK_PAWN && (src - dest == 16)){
        en_passant_sq = dest + 8;
    }
    else{
        en_passant_sq = 0;
    }
    turn = other_color(turn);
    recalculate_all_pieces();
    assert_valid();
}


void Board::recalculate_all_pieces(){
    std::array<PieceType, 6> types = {PieceType::PAWN, PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN, PieceType::KING};
    std::array<Color, 2> colors = {Color::WHITE, Color::BLACK};
    for(Color c: colors){
        std::uint64_t all_pieces_bitboard = 0;
        for(PieceType p: types){
            all_pieces_bitboard |= bitboards[to_int(to_piece(c, p))];
        }
        all_pieces[to_int(c)] = all_pieces_bitboard;
    }
}


std::uint64_t Board::get_attackers(int sq, Color attacker_color) const{
    Color ally_color = other_color(attacker_color);
    std::uint64_t allies = all_pieces[to_int(ally_color)];
    std::uint64_t enemies = all_pieces[to_int(attacker_color)];
    std::uint64_t enemy_knights = bitboards[to_int(to_piece(attacker_color, PieceType::KNIGHT))];
    std::uint64_t enemy_rooks = bitboards[to_int(to_piece(attacker_color, PieceType::ROOK))];
    std::uint64_t enemy_bishops = bitboards[to_int(to_piece(attacker_color, PieceType::BISHOP))];
    std::uint64_t enemy_queen = bitboards[to_int(to_piece(attacker_color, PieceType::QUEEN))];
    std::uint64_t enemy_pawns = bitboards[to_int(to_piece(attacker_color, PieceType::PAWN))];
    std::uint64_t enemy_king = bitboards[to_int(to_piece(attacker_color, PieceType::KING))];
    return 
        (bitboard_moves::knight(sq, allies, enemies) & enemy_knights) | 
        (bitboard_moves::bishop(sq, allies, enemies) & (enemy_bishops | enemy_queen)) | 
        (bitboard_moves::rook(sq, allies, enemies) & (enemy_rooks | enemy_queen)) | 
        (bitboard_moves::pawn_captures(sq, ally_color, enemies) & enemy_pawns) |
        (bitboard_moves::king(sq, allies, enemies) & enemy_king);
}


void Board::undo_castle(int src, int dest){
    if(src < dest){
        Color castler_color = get_color(pieces[src + 1]);
        int rook_idx = to_int(to_piece(castler_color, PieceType::ROOK));
        int king_idx = to_int(to_piece(castler_color, PieceType::KING));
        pieces[src] = pieces[src + 2];
        pieces[dest] = pieces[src + 1];
        bitboards[king_idx] >>= 2;
        bitboards[rook_idx] ^= ((1ull << dest) | (1ull << (src + 1)));
        pieces[src + 2] = Piece::EMPTY;
        pieces[src + 1] = Piece::EMPTY;
    }
    else{
        Color castler_color = get_color(pieces[src - 1]);
        int rook_idx = to_int(to_piece(castler_color, PieceType::ROOK));
        int king_idx = to_int(to_piece(castler_color, PieceType::KING));
        pieces[src] = pieces[src - 2];
        pieces[dest] = pieces[src - 1];
        bitboards[king_idx] <<= 2;
        bitboards[rook_idx] ^= ((1ull << dest) | (1ull << (src - 1)));
        pieces[src - 2] = Piece::EMPTY;
        pieces[src - 1] = Piece::EMPTY;
    }
}


void Board::undo_move_piece(int src, int dest, PieceType captured_piece_type){
    Piece attacker = pieces[dest];
    Color enemy_color = other_color(get_color(attacker));
    std::uint64_t dest_mask = 1ull << dest;
    std::uint64_t move_mask = (1ull << src) | dest_mask;
    bitboards[to_int(attacker)] ^= move_mask;
    pieces[src] = pieces[dest];
    pieces[dest] = Piece::EMPTY;
    if(captured_piece_type != PieceType::EMPTY){
        int captured_idx = to_int(to_piece(enemy_color, captured_piece_type));
        bitboards[captured_idx] |= dest_mask;
        pieces[dest] = to_piece(enemy_color, captured_piece_type);
    }
}


void Board::undo_promote(int sq, PieceType p){
    Color c = get_color(pieces[sq]);
    Piece promoted_piece = to_piece(c, p);
    Piece pawn_piece = to_piece(c, PieceType::PAWN);
    int pawn_idx = to_int(pieces[sq]);
    int promoted_idx = to_int(promoted_piece);
    std::uint64_t pos = 1ull << sq;
    bitboards[pawn_idx] ^= pos;
    bitboards[promoted_idx] ^= pos;
    pieces[sq] = pawn_piece;
}


void Board::undo_en_passant(int en_passant_sq){
    Color enemy_color = other_color(turn);
    Piece captured_piece = to_piece(turn, PieceType::PAWN);
    int captured_piece_sq = turn == Color::WHITE ? en_passant_sq + 8 : en_passant_sq - 8;
    bitboards[to_int(captured_piece)] ^= (1ull << captured_piece_sq);
    pieces[captured_piece_sq] = captured_piece;
}


void Board::undo_last_move(){
    if(prev_moves.size() == 0){
        return;
    }
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
    turn = other_color(turn);
    clock = undo_info.clock();
    recalculate_all_pieces();
    assert_valid();
}


std::uint64_t Board::get_checkers(Color c) const{
    return get_attackers(get_king_pos(c), other_color(c));
}


//assumes nonempty piece is at sq
std::uint64_t Board::pinner(int sq){
    Piece piece = pieces[sq];
    Color ally_color = get_color(piece);
    int piece_idx = to_int(piece);
    int ally_idx = to_int(ally_color);
    std::uint64_t pos = 1ull << sq;
    std::uint64_t prev_checkers = get_checkers(ally_color);
    bitboards[piece_idx] ^= pos;
    all_pieces[ally_idx] ^= pos;
    std::uint64_t after_checkers = get_checkers(ally_color);
    bitboards[piece_idx] ^= pos;
    all_pieces[ally_idx] ^= pos;
    return prev_checkers ^ after_checkers; 
}


std::uint64_t Board::get_quiets_and_captures(int sq) const{
    Piece piece = pieces[sq];
    Color ally_color = get_color(piece);
    int ally_idx = to_int(ally_color);
    int enemy_idx = to_int(other_color(ally_color));
    std::uint64_t allies = all_pieces[ally_idx];
    std::uint64_t enemies = all_pieces[enemy_idx];
    switch(get_type(piece)){
        case PieceType::ROOK: return bitboard_moves::rook(sq, allies, enemies);
        case PieceType::BISHOP: return bitboard_moves::bishop(sq, allies, enemies);
        case PieceType::QUEEN: return bitboard_moves::queen(sq, allies, enemies);
        case PieceType::KNIGHT: return bitboard_moves::knight(sq, allies, enemies);
        case PieceType::KING: return bitboard_moves::king(sq, allies, enemies);
        case PieceType::PAWN: return bitboard_moves::pawn(sq, ally_color, allies, enemies);
        case PieceType::EMPTY: return 0;
    }
    return 0;
}


std::uint64_t Board::get_promotion_row(Color c){
    return c == Color::WHITE ? 255ull << 56: 255ull;
}


std::uint64_t Board::get_en_passant_row(Color c){
    using namespace bitboard;
    return c == Color::WHITE ? slide<S>(slide<S>(defaults::bitboards_init[to_int(Piece::BLACK_PAWN)])) : slide<N>(slide<N>(defaults::bitboards_init[to_int(Piece::WHITE_PAWN)]));
}


//An exception will occur at get_checkers if the user tries to move a piece that is already attacking the opponent king. 
//This is because when testing all possible moves that piece may make for legality, 
//one of those moves will involve capturing the opponent king. 
//It is not possible to determine if the opponent king is in check, if it doesn't exist on the board!
//With turns-based moves, this exception will never occur
std::uint64_t Board::get_legal_quiets_and_captures(int sq){
    Piece piece = pieces[sq];
    int piece_idx = to_int(piece);
    Color ally_color = get_color(piece);
    Color enemy_color = other_color(ally_color);
    PieceType ally_type = get_type(piece);
    //cannot block a double check with a non-king piece
    if(piece == Piece::EMPTY || (std::popcount(get_checkers(ally_color)) == 2 && ally_type != PieceType::KING)){
        return 0;
    }
    std::uint64_t attacks = get_quiets_and_captures(sq);
    if(ally_type == PieceType::KING){
        std::uint64_t legal_moves = 0;
        bitboards[piece_idx] = 0;
        all_pieces[to_int(ally_color)] ^= (1ull << sq);
        while(attacks){
            int dest = std::countr_zero(attacks);
            std::uint64_t dest_mask = 1ull << dest;
            if(get_attackers(dest, enemy_color) == 0){
                legal_moves |= dest_mask;
            }
            attacks ^= dest_mask;
        }
        bitboards[piece_idx] = 1ull << sq;
        all_pieces[to_int(ally_color)] ^= (1ull << sq);
        recalculate_all_pieces();
        return legal_moves;
    }
    else{
        //if this point reached, piece is not king and there is at most one checker
        std::uint64_t pin = pinner(sq);
        if(pin != 0 && in_check(ally_color)){
            return 0;
        }
        else if(pin != 0){
            int pin_sq = std::countr_zero(pin);
            int king_idx = to_int(to_piece(ally_color, PieceType::KING));
            int king_sq = std::countr_zero(bitboards[king_idx]);
            std::uint64_t line_of_attack = tables::attack_from_piece_to_king[pin_sq][king_sq];
            return line_of_attack & attacks;
        }
        else if(in_check(ally_color)){
            int checker_sq = std::countr_zero(get_checkers(ally_color));
            int king_idx = to_int(to_piece(ally_color, PieceType::KING));
            int king_sq = std::countr_zero(bitboards[king_idx]);
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
    if(get_checkers(c) != 0) return moves;
    std::uint64_t occ = all_pieces[to_int(Color::WHITE)] | all_pieces[to_int(Color::BLACK)];
    if(c == Color::WHITE){
        constexpr std::uint64_t white_left_castle_mask = (1ull << 4) | (1ull << 5) & (1ull << 6);
        constexpr std::uint64_t white_right_castle_mask = (1ull << 1) | (1ull << 2);
        if((castle_rights & white_left_castle_allowed_flag) && !(occ & white_left_castle_mask) && !get_attackers(4, Color::BLACK) && !get_attackers(5, Color::BLACK)){
            moves |= (1ull << 7);
        }
        if((castle_rights & white_left_castle_allowed_flag) && !(occ & white_right_castle_mask) && !get_attackers(2, Color::BLACK) && !get_attackers(1, Color::BLACK)){
            moves |= 1;
        }
    }
    else{
        constexpr std::uint64_t black_left_castle_mask = (1ull << 60) | (1ull << 61) | (1ull << 62);
        constexpr std::uint64_t black_right_castle_mask = (1ull << 57) | (1ull << 58);
        if((castle_rights & black_left_castle_allowed_flag) && !(occ & black_left_castle_mask) && !get_attackers(60, Color::WHITE) && !get_attackers(61, Color::WHITE)){
            moves |= (1ull << 63);
        }
        if((castle_rights & black_right_castle_allowed_flag) && !(occ & black_right_castle_mask) && !get_attackers(57, Color::WHITE) && !get_attackers(58, Color::WHITE)){
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
    Piece piece = pieces[sq];
    if(piece == Piece::EMPTY){
        return legal_moves;
    }
    Color ally_color = get_color(piece);
    if(ally_color != turn){
        return legal_moves;
    }
    PieceType ally_type = get_type(piece);
    int enemy_idx = to_int(other_color(ally_color));
    std::uint64_t quiets_and_captures = get_legal_quiets_and_captures(sq);
    std::uint64_t enemies = all_pieces[enemy_idx];
    std::uint64_t en_passant = ((1ull << sq) & get_en_passant_row(ally_color)) ? bitboard_moves::pawn_en_passant(sq, ally_color, en_passant_sq) : 0;
    std::uint64_t castles = ally_type == PieceType::KING ? get_castle_moves(ally_color) : 0;
    while(quiets_and_captures){
        int dest = std::countr_zero(quiets_and_captures);
        std::uint64_t dest_mask = 1ull << dest;
        std::uint16_t flags = 0;
        if(dest_mask & enemies){
            flags |= Move::capture_flag;
        }
        if((dest_mask & get_promotion_row(ally_color)) && ally_type == PieceType::PAWN){
            flags |= Move::promotion_flag;
            legal_moves.add_move(Move(sq, dest, flags, PieceType::QUEEN));
            legal_moves.add_move(Move(sq, dest, flags, PieceType::KNIGHT));
            legal_moves.add_move(Move(sq, dest, flags, PieceType::ROOK));
            legal_moves.add_move(Move(sq, dest, flags, PieceType::BISHOP));
        } 
        else{
            legal_moves.add_move(Move(sq, dest, flags));
        }
        quiets_and_captures ^= (1ull << dest);
    }
    if(en_passant){
        legal_moves.add_move(Move(sq, en_passant_sq, Move::en_passant_flag));
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


std::string Board::layout() const{
    std::string layout;
    for(int i = 63; i > -1; i--){
        switch(pieces[i]){
            case Piece::WHITE_PAWN:
                layout.push_back('P');
                break;
            case Piece::WHITE_ROOK:
                layout.push_back('R');
                break;
            case Piece::WHITE_KNIGHT:
                layout.push_back('N');
                break;
            case Piece::WHITE_BISHOP:
                layout.push_back('B');
                break;
            case Piece::WHITE_KING:
                layout.push_back('K');
                break;
            case Piece::WHITE_QUEEN:
                layout.push_back('Q');
                break;
            case Piece::BLACK_PAWN:
                layout.push_back('p');
                break;
            case Piece::BLACK_ROOK:
                layout.push_back('r');
                break;
            case Piece::BLACK_KNIGHT:
                layout.push_back('n');
                break;
            case Piece::BLACK_BISHOP:
                layout.push_back('b');
                break;
            case Piece::BLACK_QUEEN:
                layout.push_back('q');
                break;
            case Piece::BLACK_KING:
                layout.push_back('k');
                break;
            default:
                layout += "·";
        }
        if(i % 8 == 0){
            layout += "\n";
        }
    }
    return layout;
}


void Board::assert_valid() const{
    constexpr std::array<Piece, 12> piece_enum = {
        Piece::WHITE_ROOK, Piece::WHITE_KNIGHT, Piece::WHITE_BISHOP, Piece::WHITE_QUEEN, Piece::WHITE_KING, Piece::WHITE_PAWN, 
        Piece::BLACK_PAWN, Piece::BLACK_ROOK, Piece::BLACK_KNIGHT, Piece::BLACK_BISHOP, Piece::BLACK_QUEEN, Piece::BLACK_KING
    };
    for(int i = 0; i < 64; i++){
        Piece p = pieces[i];
        bool valid = true;
        if(p == Piece::EMPTY){
            std::uint64_t dest = 1ull << i;
            for(Piece occ: piece_enum){
                if(occ == p){
                    assert(((bitboards[to_int(occ)] & dest) && "Occupancy board must be turned on at this bit"));
                }
                else{
                    assert((!(bitboards[to_int(occ)] & dest) && "Occupancy board must be turned off at this bit"));
                }
            }
        }
    }
}



void Board::reset(){
    using namespace defaults;
    bitboards = bitboards_init;
    all_pieces = all_pieces_init;
    pieces = pieces_init;
    en_passant_sq = 0;
    castle_rights = castle_rights_init;
    clock = 0;
    turn = Color::WHITE;
}