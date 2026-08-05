#include "board.hpp"
#include "move.hpp"
#include "bitboard_moves.hpp"
#include "undo_move.hpp"
#include "perft_results.hpp"
#include <bit>
#include <charconv>
#include <utility>
#include <cassert>
#include <cctype>
#include <algorithm>


namespace{
    inline constexpr std::uint64_t white_right_all_mask = 15;
    inline constexpr std::uint64_t white_left_all_mask = (1ull << 3) | (1ull << 4) | (1ull << 5) | (1ull << 7);
    inline constexpr std::uint64_t black_right_all_mask = (1ull << 56) | (1ull << 57) | (1ull << 58) | (1ull << 59);
    inline constexpr std::uint64_t black_left_all_mask = (1ull << 59) | (1ull << 60) | (1ull << 61) | (1ull << 63);
    inline const std::string piece_chars = "PRNBQKprnbqk";
    inline const std::string files = "abcdefgh";
    inline const std::string ranks = "12345678";
}


Board::Board(){
    reset();
}


int Board::get_king_pos(Color c) const{
    return c == Color::WHITE ? std::countr_zero(bitboards[Piece::WHITE_KING]) : 
                               std::countr_zero(bitboards[Piece::BLACK_KING]);
}


void Board::do_castle(int src, int dest){
    Piece rook = to_piece(turn, PieceType::ROOK);
    Piece king = to_piece(turn, PieceType::KING);
    if(src < dest){
        bitboards[king] <<= 2;
        bitboards[rook] ^= ((1ull << dest) | (1ull << (src + 1)));
        pieces[src + 2] = king;
        pieces[src + 1] = rook;
        //all_pieces[turn] ^= (turn == Color::WHITE ? white_left_all_mask : black_left_all_mask);
    }
    else{
        bitboards[king] >>= 2;
        bitboards[rook] ^= ((1ull << dest) | (1ull << (src - 1)));
        pieces[src - 2] = king;
        pieces[src - 1] = rook;
        //all_pieces[turn] ^= (turn == Color::WHITE ? white_right_all_mask : black_right_all_mask);
    }
    pieces[src] = Piece::EMPTY;
    pieces[dest] = Piece::EMPTY;
    castle_rights &= ~(turn == Color::WHITE ? (white_left_castle_allowed_flag | white_right_castle_allowed_flag) : 
                                                     (black_left_castle_allowed_flag | black_right_castle_allowed_flag));
}


void Board::move_piece(int src, int dest){
    Piece attacker = pieces[src];
    Piece captured = pieces[dest];
    std::uint64_t dest_mask = 1ull << dest;
    std::uint64_t move_mask = (1ull << src) | dest_mask;
    bitboards[attacker] ^= move_mask;
    pieces[src] = Piece::EMPTY;
    //all_pieces[turn] ^= move_mask;
    if(captured != Piece::EMPTY){
        bitboards[captured] ^= dest_mask;
        all_pieces[other_color(turn)] ^= dest_mask;
    }
    pieces[dest] = attacker;
    PieceType attacker_type = get_type(attacker);
    if(attacker_type == PieceType::KING){
        castle_rights &= ~(turn == Color::WHITE ? (white_left_castle_allowed_flag | white_right_castle_allowed_flag) : 
                                                     (black_left_castle_allowed_flag | black_right_castle_allowed_flag));
    }
    else if(src == 0 || dest == 0) castle_rights &= ~(white_right_castle_allowed_flag);
    else if(src == 7 || dest == 7) castle_rights &= ~(white_left_castle_allowed_flag);
    else if(src == 56 || dest == 56) castle_rights &= ~(black_right_castle_allowed_flag);
    else if(src == 63 || dest == 63) castle_rights &= ~(black_left_castle_allowed_flag);
}


void Board::promote(int sq, PieceType p){
    std::uint64_t pos = 1ull << sq;
    Piece pawn = pieces[sq];
    Piece promoted_piece = to_piece(get_color(pawn), p);
    bitboards[pawn] ^= pos;
    bitboards[promoted_piece] ^= pos;
    //all_pieces[turn] ^= pos;
    //all_pieces[other_color(turn)] ^= pos;
    pieces[sq] = promoted_piece;
}


void Board::do_en_passant(int en_passant_sq){
    Color enemy_color = other_color(turn);
    Piece pawn = to_piece(enemy_color, PieceType::PAWN);
    int captured_piece_sq = turn == Color::WHITE ? en_passant_sq - 8 : en_passant_sq + 8;
    std::uint64_t captured_mask = 1ull << captured_piece_sq;
    bitboards[pawn] ^= captured_mask;
    //all_pieces[enemy_color] ^= captured_mask;
    pieces[captured_piece_sq] = Piece::EMPTY;
    en_passant_sq = 0;
}


void Board::make_move(Move move){
    int src = move.src();
    int dest = move.dest();
    Piece moved = pieces[src];
    Color enemy_color = other_color(turn);
    prev_moves.emplace(move, UndoMove(en_passant_sq, get_type(pieces[dest]), castle_rights, clock, pinned, checkers));
    if(move.is_castle()){
        do_castle(src, dest);
    }
    else{
        move_piece(src, dest);
        if(move.is_promotion()){
            promote(dest, move.promoted_type());
        }
        else if(move.is_en_passant()){
            do_en_passant(en_passant_sq);
        }
    }
    if(get_type(moved) == PieceType::PAWN){
        clock = 0;
        if(dest - src == 16 && src > 7 && src < 16){
            en_passant_sq = src + 8;
        }
        else if(src - dest == 16 && src > 47 && src < 56){
            en_passant_sq = dest + 8;
        }
        else{
            en_passant_sq = 0;
        }
    }
    else{
        en_passant_sq = 0;
        if(move.is_capture()){
            clock = 0;
        }
        else{
            clock++;
        }
    }
    turn = enemy_color;
    recalculate_all_pieces();
    recalculate_pinned(turn);
    checkers = get_checkers(turn);
    //assert_valid();
    //Must omit the following line in debug builds, since en passant move gen relies on checking if move leaves ally king in check to determine legality
    //assert(!in_check(other_color(enemy_color)) && "Cannot leave ally king in check"); 
}


void Board::recalculate_all_pieces(){
    all_pieces[Color::WHITE] = bitboards[Piece::WHITE_PAWN] | bitboards[Piece::WHITE_ROOK] | bitboards[Piece::WHITE_KNIGHT] | bitboards[Piece::WHITE_BISHOP] | bitboards[Piece::WHITE_QUEEN] | bitboards[Piece::WHITE_KING];
    all_pieces[Color::BLACK] = bitboards[Piece::BLACK_PAWN] | bitboards[Piece::BLACK_ROOK] | bitboards[Piece::BLACK_KNIGHT] | bitboards[Piece::BLACK_BISHOP] | bitboards[Piece::BLACK_QUEEN] | bitboards[Piece::BLACK_KING];
}


std::uint64_t Board::get_attackers(int sq, Color attacker_color) const{
    std::uint64_t allies = all_pieces[turn];
    std::uint64_t enemies = all_pieces[attacker_color];
    std::uint64_t enemy_queen = bitboards[to_piece(attacker_color, PieceType::QUEEN)];
    return 
        (bitboard_moves::knight(sq, allies, enemies) & bitboards[to_piece(attacker_color, PieceType::KNIGHT)]) | 
        (bitboard_moves::bishop(sq, allies, enemies) & (bitboards[to_piece(attacker_color, PieceType::BISHOP)] | enemy_queen)) | 
        (bitboard_moves::rook(sq, allies, enemies) & (bitboards[to_piece(attacker_color, PieceType::ROOK)] | enemy_queen)) | 
        (bitboard_moves::pawn_captures(sq, turn, enemies) & bitboards[to_piece(attacker_color, PieceType::PAWN)]) |
        (bitboard_moves::king(sq, allies, enemies) & bitboards[to_piece(attacker_color, PieceType::KING)]);
}


bool Board::is_attacked(int sq, Color attacker_color) const{
    Color defender_color = other_color(attacker_color);
    std::uint64_t allies = all_pieces[defender_color];
    std::uint64_t enemies = all_pieces[attacker_color];
    std::uint64_t enemy_queen = bitboards[to_piece(attacker_color, PieceType::QUEEN)];
    if(bitboard_moves::rook(sq, allies, enemies) & (bitboards[to_piece(attacker_color, PieceType::ROOK)] | enemy_queen)){
        return true;
    } 
    else if(bitboard_moves::bishop(sq, allies, enemies) & (bitboards[to_piece(attacker_color, PieceType::BISHOP)] | enemy_queen)){
        return true;
    }
    else if(bitboard_moves::knight(sq, allies, enemies) & bitboards[to_piece(attacker_color, PieceType::KNIGHT)]){
        return true;
    }
    else if(bitboard_moves::pawn_captures(sq, defender_color, enemies) & bitboards[to_piece(attacker_color, PieceType::PAWN)]){
        return true;
    }
    else if(bitboard_moves::king(sq, allies, enemies) & bitboards[to_piece(attacker_color, PieceType::KING)]){
        return true;
    }
    return false;
}


void Board::undo_castle(int src, int dest){
    if(src < dest){
        pieces[src] = pieces[src + 2];
        pieces[dest] = pieces[src + 1];
        bitboards[to_piece(turn, PieceType::KING)] >>= 2;
        bitboards[to_piece(turn, PieceType::ROOK)] ^= ((1ull << dest) | (1ull << (src + 1)));
        pieces[src + 2] = Piece::EMPTY;
        pieces[src + 1] = Piece::EMPTY;
        //all_pieces[turn] ^= (turn == Color::WHITE ? white_left_all_mask : black_left_all_mask);
    }
    else{
        pieces[src] = pieces[src - 2];
        pieces[dest] = pieces[src - 1];
        bitboards[to_piece(turn, PieceType::KING)] <<= 2;
        bitboards[to_piece(turn, PieceType::ROOK)] ^= ((1ull << dest) | (1ull << (src - 1)));
        pieces[src - 2] = Piece::EMPTY;
        pieces[src - 1] = Piece::EMPTY;
        //all_pieces[turn] ^= (turn == Color::WHITE ? white_right_all_mask : black_right_all_mask);
    }
}


void Board::undo_move_piece(int src, int dest, PieceType captured_piece_type){
    Piece attacker = pieces[dest];
    std::uint64_t dest_mask = 1ull << dest;
    std::uint64_t move_mask = (1ull << src) | dest_mask;
    bitboards[attacker] ^= move_mask;
    pieces[src] = pieces[dest];
    pieces[dest] = Piece::EMPTY;
    //all_pieces[turn] ^= move_mask;
    if(captured_piece_type != PieceType::EMPTY){
        Color defender_color = other_color(turn);
        bitboards[to_piece(defender_color, captured_piece_type)] |= dest_mask;
        pieces[dest] = to_piece(defender_color, captured_piece_type);
        //all_pieces[defender_color] ^= dest_mask;
    }
}


void Board::undo_promote(int sq){
    Piece promoted_piece = pieces[sq];
    Piece pawn = to_piece(turn, PieceType::PAWN);
    std::uint64_t pos = 1ull << sq;
    bitboards[pawn] ^= pos;
    bitboards[promoted_piece] ^= pos;
    pieces[sq] = pawn;
    //all_pieces[turn] ^= pos;
    //all_pieces[other_color(turn)] ^= pos;
}


void Board::undo_en_passant(int en_passant_sq){
    Color captured_color = other_color(turn);
    Piece captured_piece = to_piece(captured_color, PieceType::PAWN);
    int captured_piece_sq = turn == Color::WHITE ? en_passant_sq - 8 : en_passant_sq + 8;
    std::uint64_t captured_mask = (1ull << captured_piece_sq);
    bitboards[captured_piece] ^= captured_mask;
    //all_pieces[captured_color] ^= captured_mask;
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
    turn = other_color(turn);
    if(move.is_castle()){
        undo_castle(src, dest);
    }
    else{
        if(move.is_promotion()){
            undo_promote(dest);
        }
        undo_move_piece(src, dest, undo_info.captured_piece_type());
        if(move.is_en_passant()){
            undo_en_passant(undo_info.en_passant_sq());
        }
    }
    en_passant_sq = undo_info.en_passant_sq();
    castle_rights = undo_info.castle_rights();
    pinned = undo_info.prev_enemy_pinned();
    checkers = undo_info.prev_checkers();
    clock = undo_info.clock();
    recalculate_all_pieces();
    //assert_valid();
}


std::uint64_t Board::get_checkers(Color c) const{
    return get_attackers(get_king_pos(c), other_color(c));
}


std::uint64_t Board::get_quiets_and_captures(int sq) const{
    Piece piece = pieces[sq];
    Color ally_color = get_color(piece);
    std::uint64_t allies = all_pieces[ally_color];
    std::uint64_t enemies = all_pieces[other_color(ally_color)];
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
    return c == Color::WHITE ? slide<S>(slide<S>(tables::defaults::bitboards_init[Piece::BLACK_PAWN])) : 
                               slide<N>(slide<N>(tables::defaults::bitboards_init[Piece::WHITE_PAWN]));
}


void Board::recalculate_pinned(Color c){
    if(c == Color::WHITE){
        recalculate_pinned<Color::WHITE>();
    }
    else{
        recalculate_pinned<Color::BLACK>();
    }
}


//An exception will occur at get_checkers if the user tries to move a piece that is already attacking the opponent king. 
//This is because when testing all possible moves that piece may make for legality, 
//one of those moves will involve capturing the opponent king. 
//It is not possible to determine if the opponent king is in check, if it doesn't exist on the board!
//With turns-based moves, this exception will never occur
std::uint64_t Board::get_legal_quiets_and_captures(int sq){
    Piece piece = pieces[sq];
    Color ally_color = get_color(piece);
    Color enemy_color = other_color(ally_color);
    PieceType ally_type = get_type(piece);
    //cannot block a double check with a non-king piece
    if(piece == Piece::EMPTY || (std::popcount(checkers) == 2 && ally_type != PieceType::KING)){
        return 0;
    }
    std::uint64_t attacks = get_quiets_and_captures(sq);
    if(ally_type == PieceType::KING){
        std::uint64_t legal_moves = 0;
        bitboards[piece] = 0;
        all_pieces[ally_color] ^= (1ull << sq);
        while(attacks){
            int dest = std::countr_zero(attacks);
            std::uint64_t dest_mask = 1ull << dest;
            if(!is_attacked(dest, enemy_color)){
                legal_moves |= dest_mask;
            }
            attacks ^= dest_mask;
        }
        bitboards[piece] = 1ull << sq;
        all_pieces[ally_color] ^= (1ull << sq);
        return legal_moves;
    }
    else{
        //if this point reached, piece is not king and there is at most one checker
        using namespace tables::pins;
        bool is_checked = (checkers != 0);
        int king_sq = get_king_pos(ally_color);
        bool is_pinned = (pinned & (1ull << sq));
        if(is_pinned && is_checked){
            return 0;
        }
        else if(is_pinned){
            return pin_rays[king_sq][sq] & attacks;
        }
        else if(is_checked){
            int checker_pos = std::countr_zero(checkers);
            PieceType checker_type = get_type(pieces[checker_pos]);
            std::uint64_t check_eliminaters = checkers;
            if(checker_type == PieceType::BISHOP || checker_type == PieceType::ROOK || checker_type == PieceType::QUEEN){
                check_eliminaters |= between[king_sq][checker_pos];
            }
            return attacks & check_eliminaters;
        }
        else{
            return attacks;
        }
    }
}


std::uint64_t Board::get_legal_castle_moves(Color c){
    std::uint64_t moves = 0;
    if(in_check(c)) return moves;
    std::uint64_t occ = all_pieces[Color::WHITE] | all_pieces[Color::BLACK];
    Color enemy_color = other_color(c);
    if(c == Color::WHITE){
        constexpr std::uint64_t white_left_castle_mask = (1ull << 4) | (1ull << 5) | (1ull << 6);
        constexpr std::uint64_t white_right_castle_mask = (1ull << 1) | (1ull << 2);
        if((castle_rights & white_left_castle_allowed_flag) && !(occ & white_left_castle_mask) && !is_attacked(4, enemy_color) && !is_attacked(5, enemy_color)){
            moves |= (1ull << 7);
        }
        if((castle_rights & white_right_castle_allowed_flag) && !(occ & white_right_castle_mask) && !is_attacked(2, enemy_color) && !is_attacked(1, enemy_color)){
            moves |= 1;
        }
    }
    else{
        constexpr std::uint64_t black_left_castle_mask = (1ull << 60) | (1ull << 61) | (1ull << 62);
        constexpr std::uint64_t black_right_castle_mask = (1ull << 57) | (1ull << 58);
        if((castle_rights & black_left_castle_allowed_flag) && !(occ & black_left_castle_mask) && !is_attacked(60, enemy_color) && !is_attacked(61, enemy_color)){
            moves |= (1ull << 63);
        }
        if((castle_rights & black_right_castle_allowed_flag) && !(occ & black_right_castle_mask) && !is_attacked(57, enemy_color) && !is_attacked(58, enemy_color)){
            moves |= (1ull << 56);
        }
    }
    return moves;
}


std::uint64_t Board::get_legal_en_passant_moves(int sq){
    Piece pawn = pieces[sq];
    Color c = get_color(pawn);
    if(get_type(pawn) != PieceType::PAWN || ((1ull << sq) & get_en_passant_row(c)) == 0){
        return 0;
    }
    std::uint64_t move = bitboard_moves::pawn_en_passant(sq, c, en_passant_sq);
    if(move == 0){
        return 0;
    }
    make_move(Move(sq, std::countr_zero(move), Move::en_passant_flag | Move::capture_flag));
    bool results_in_check = in_check(c);
    undo_last_move();
    return results_in_check ? 0 : move;
}


BoardState Board::get_board_state(){
    return BoardState(*this);
}


void Board::populate_legal_moves(int sq, MoveList& list){
    Piece piece = pieces[sq];
    if(piece == Piece::EMPTY){
        return;
    }
    Color ally_color = get_color(piece);
    if(ally_color != turn){
        return;
    }
    PieceType ally_type = get_type(piece);
    Color enemy_color = other_color(ally_color);
    std::uint64_t quiets_and_captures = get_legal_quiets_and_captures(sq);
    std::uint64_t enemies = all_pieces[enemy_color];
    std::uint64_t en_passant = get_legal_en_passant_moves(sq);
    std::uint64_t castles = ally_type == PieceType::KING ? get_legal_castle_moves(ally_color) : 0;
    // keep in mind for future: if a pawn has one promotion move it can currently perform, then all of its moves must be promotions also
    while(quiets_and_captures){
        int dest = std::countr_zero(quiets_and_captures);
        std::uint64_t dest_mask = 1ull << dest;
        Move::MoveFlag flags = Move::empty_flag;
        if(dest_mask & enemies){
            flags |= Move::capture_flag;
        }
        if((dest_mask & get_promotion_row(ally_color)) && ally_type == PieceType::PAWN){
            flags |= Move::promotion_flag;
            list.add_move(Move(sq, dest, flags, PieceType::QUEEN));
            list.add_move(Move(sq, dest, flags, PieceType::KNIGHT));
            list.add_move(Move(sq, dest, flags, PieceType::BISHOP));
            list.add_move(Move(sq, dest, flags, PieceType::ROOK));
        }
        else{
            list.add_move(Move(sq, dest, flags));
        }
        quiets_and_captures ^= (1ull << dest);
    }
    if(en_passant){
        list.add_move(Move(sq, en_passant_sq, Move::en_passant_flag | Move::capture_flag));
    }
    while(castles){
        int dest = std::countr_zero(castles);
        list.add_move(Move(sq, dest, Move::castle_flag));
        castles ^= (1ull << dest);
    }
}


void Board::populate_legal_moves(MoveList& list){
    for(int sq = 0; sq < 64; sq++){
        populate_legal_moves(sq, list);
    }
}


bool Board::in_check(Color c){
    return c == turn ? checkers != 0 : is_attacked(get_king_pos(c), other_color(c));
}


bool Board::turn_color_in_check(){
    return in_check(turn);
}

std::string Board::layout() const{
    std::string layout;
    for(int i = 63; i > -1; i--){
        switch(pieces[i]){
            case Piece::WHITE_PAWN:
                layout += "\u2659 "; break;
            case Piece::WHITE_ROOK:
                layout += "\u2656 "; break;
            case Piece::WHITE_KNIGHT:
                layout += "\u2658 "; break;
            case Piece::WHITE_BISHOP:
                layout += "\u2657 "; break;
            case Piece::WHITE_KING:
                layout += "\u2654 "; break;
            case Piece::WHITE_QUEEN:
                layout += "\u2655 "; break;
            case Piece::BLACK_PAWN:
                layout += "\u265F "; break;
            case Piece::BLACK_ROOK:
                layout += "\u265C "; break;
            case Piece::BLACK_KNIGHT:
                layout += "\u265E "; break;
            case Piece::BLACK_BISHOP:
                layout += "\u265D "; break;
            case Piece::BLACK_QUEEN:
                layout += "\u265B "; break;
            case Piece::BLACK_KING:
                layout += "\u265A "; break;
            default:
                layout += "\u00B7 ";
        }
        if(i % 8 == 0){
            layout += "\n";
        }
    }
    return layout;
}


void Board::assert_valid(){
    constexpr std::array<Piece, 12> piece_enum = {
        Piece::WHITE_ROOK, Piece::WHITE_KNIGHT, Piece::WHITE_BISHOP, Piece::WHITE_QUEEN, Piece::WHITE_KING, Piece::WHITE_PAWN, 
        Piece::BLACK_PAWN, Piece::BLACK_ROOK, Piece::BLACK_KNIGHT, Piece::BLACK_BISHOP, Piece::BLACK_QUEEN, Piece::BLACK_KING
    };
    std::uint64_t white_pieces = all_pieces[Color::WHITE];
    std::uint64_t black_pieces = all_pieces[Color::BLACK];
    for(int i = 0; i < 64; i++){
        Piece p = pieces[i];
        bool valid = true;
        if(p == Piece::EMPTY){
            std::uint64_t dest = 1ull << i;
            for(Piece occ: piece_enum){
                if(occ == p){
                    assert(((bitboards[occ] & dest) && "Occupancy board must be turned on at this bit"));
                }
                else{
                    assert((!(bitboards[occ] & dest) && "Occupancy board must be turned off at this bit"));
                }
            }
        }
    }
}


void Board::populate_perft(int depth, PerftResults& stats){
    if(depth == 0){
        stats.nodes = 1;
        return;
    }
    MoveList move_list;
    populate_legal_moves(move_list);
    for(int i = 0; i < move_list.size(); i++){
        Move move = move_list[i];
        make_move(move);
        if(depth == 1){
            stats.nodes++;
            if(move.is_promotion()){
                stats.promotions++;
            }
            if(move.is_capture()){
                stats.captures++;
            }
            if(move.is_en_passant()){
                stats.en_passants++;
            }
            if(move.is_castle()){
                stats.castles++;
            }
            if(turn_color_in_check()){
                stats.checks++;
            }
        } 
        else{
            populate_perft(depth - 1, stats);
        }
        undo_last_move();
    }
}


std::expected<std::array<std::string_view, 6>, FENError> Board::parse_FEN(const std::string& str){
    std::array<std::string_view, 6> sections;
    auto not_whitespace = [](unsigned char c){return !std::isspace(c);};
    auto is_whitespace = [](unsigned char c){return std::isspace(c);};
    auto start = str.begin();
    for(int i = 0; i < 6; i++){
        start = std::find_if(start, str.end(), not_whitespace);
        if(start == str.end()){
            return std::unexpected(FENError::MISSING_SECTION);
        }
        auto end = std::find_if(start, str.end(), is_whitespace);
        sections[i] = std::string_view{start, end};
        start = end;
    }
    if(std::find_if(start, str.end(), not_whitespace) != str.end()) return std::unexpected(FENError::EXTRA_SECTION);

    auto board = sections[0];
    int cur_row_count = 1;
    int cur_col_count = 0;
    bool white_king_on_board = false;
    bool black_king_on_board = false;
    for(char p: board){
        if(p == '0') return std::unexpected(FENError::ZERO_IN_BOARD);
        if(p == '/'){
            if(cur_col_count != 8){
                return std::unexpected(FENError::NOT_ENOUGH_COLUMNS);
            }
            else{
                cur_col_count = 0;
                cur_row_count++;
            }
        }
        else if(std::isdigit(p)){
            cur_col_count += static_cast<int>(p - '0');
        }
        else if(piece_chars.contains(p)){
            cur_col_count++;
            if(p == 'K'){
                if(white_king_on_board) return std::unexpected(FENError::DUPLICATE_KING);
                else white_king_on_board = true;
            }
            else if(p == 'k'){
                if(black_king_on_board) return std::unexpected(FENError::DUPLICATE_KING);
                else black_king_on_board = true;
            }
        }
        else return std::unexpected(FENError::INVALID_CHARACTER_IN_ROW);
    }
    if(cur_row_count != 8) return std::unexpected(FENError::NOT_ENOUGH_ROWS);
    else if(cur_row_count != 8) return std::unexpected(FENError::NOT_ENOUGH_COLUMNS);
    else if(!(white_king_on_board && black_king_on_board)) return std::unexpected(FENError::MISSING_KING);

    auto active_color = sections[1];
    if(active_color.size() != 1 || (active_color[0] != 'w' && active_color[0] != 'b')) return std::unexpected(FENError::INVALID_ACTIVE_COLOR);

    auto castle_rights = sections[2];
    if(castle_rights != "-"){
        bool found_K = false, found_Q = false, found_k = false, found_q = false;
        for(char c: castle_rights){
            if(c == 'K'){
                if(found_K) return std::unexpected(FENError::DUPLICATE_CASTLE_RIGHTS);
                else found_K = true;
            }
            else if(c == 'Q'){
                if(found_Q) return std::unexpected(FENError::DUPLICATE_CASTLE_RIGHTS);
                else found_Q = true;
            }
            else if(c == 'k'){
                if(found_k) return std::unexpected(FENError::DUPLICATE_CASTLE_RIGHTS);
                else found_k = true;
            }
            else if(c == 'q'){
                if(found_q) return std::unexpected(FENError::DUPLICATE_CASTLE_RIGHTS);
                else found_q = true;
            }
            else return std::unexpected(FENError::INVALID_CASTLE_RIGHTS);
        }
    }

    auto en_passant_sq = sections[3];
    if(en_passant_sq != "-"){
        int en_passant_sq_num = 0;
        auto [ptr1, err1] = std::from_chars(en_passant_sq.data(), en_passant_sq.data() + en_passant_sq.size(), en_passant_sq_num);
        if(err1 != std::errc{}) return std::unexpected(FENError::INVALID_EN_PASSANT_SQ);
        else if(en_passant_sq_num < 0 || en_passant_sq_num > 63) return std::unexpected(FENError::EN_PASSANT_SQ_OUT_OF_BOUNDS);
    }

    auto clock = sections[4];
    int clock_num = 0;
    auto [ptr1, err1] = std::from_chars(clock.data(), clock.data() + clock.size(), clock_num);
    if(err1 != std::errc{} || clock_num < 0) return std::unexpected(FENError::INVALID_HALF_MOVE_CLOCK);

    auto full_moves = sections[4];
    int moves_num = 0;
    auto [ptr2, err2] = std::from_chars(full_moves.data(), full_moves.data() + full_moves.size(), moves_num);
    if(err2 != std::errc{} || moves_num < 0) return std::unexpected(FENError::INVALID_NUM_MOVES);

    return sections;
}


std::expected<void, FENError> Board::load_FEN(const std::string& str){
    auto res = parse_FEN(str);
    if(!res.has_value()) return std::unexpected(res.error());
    const auto& sections = res.value();
    
    prev_moves = std::stack<History>();
    auto board_FEN = sections[0];
    int sq = 63;
    std::uint64_t trav = 1ull << 63;
    bitboards.fill(0);
    pieces.fill(Piece::EMPTY);
    for(char p: board_FEN){
        if(p == '/') continue;
        if(std::isdigit(p)){
            trav >>= static_cast<int>(p - '0');
            sq -= static_cast<int>(p - '0');
            continue;
        }
        switch(p){
            case 'P': bitboards[Piece::WHITE_PAWN] |= trav; pieces[sq] = Piece::WHITE_PAWN; break;
            case 'R': bitboards[Piece::WHITE_ROOK] |= trav; pieces[sq] = Piece::WHITE_ROOK; break;
            case 'N': bitboards[Piece::WHITE_KNIGHT] |= trav; pieces[sq] = Piece::WHITE_KNIGHT;  break;
            case 'B': bitboards[Piece::WHITE_BISHOP] |= trav; pieces[sq] = Piece::WHITE_BISHOP;  break;
            case 'Q': bitboards[Piece::WHITE_QUEEN] |= trav; pieces[sq] = Piece::WHITE_QUEEN;  break;
            case 'K': bitboards[Piece::WHITE_KING] |= trav; pieces[sq] = Piece::WHITE_KING;  break;
            case 'p': bitboards[Piece::BLACK_PAWN] |= trav; pieces[sq] = Piece::BLACK_PAWN;  break;
            case 'r': bitboards[Piece::BLACK_ROOK] |= trav; pieces[sq] = Piece::BLACK_ROOK;  break;
            case 'n': bitboards[Piece::BLACK_KNIGHT] |= trav; pieces[sq] = Piece::BLACK_KNIGHT;  break;
            case 'b': bitboards[Piece::BLACK_BISHOP] |= trav; pieces[sq] = Piece::BLACK_BISHOP;  break;
            case 'q': bitboards[Piece::BLACK_QUEEN] |= trav; pieces[sq] = Piece::BLACK_QUEEN;  break;
            case 'k': bitboards[Piece::BLACK_KING] |= trav; pieces[sq] = Piece::BLACK_KING;  break;
            default: break;
        }
        trav >>= 1;
        sq--;
    }
    recalculate_all_pieces();
    assert_valid();

    if(sections[1] == "w"){
        turn = Color::WHITE;
        recalculate_pinned<Color::WHITE>();
    }
    else{
        turn = Color::BLACK;
        recalculate_pinned<Color::BLACK>();
    }

    castle_rights = 0;
    auto castle_rights_FEN = sections[2];
    if(castle_rights_FEN != "-"){
        for(char castle_right: castle_rights_FEN){
            switch(castle_right){
                case 'K': castle_rights |= white_right_castle_allowed_flag; break;
                case 'Q': castle_rights |= white_left_castle_allowed_flag; break;
                case 'k': castle_rights |= black_right_castle_allowed_flag; break;
                case 'q': castle_rights |= black_left_castle_allowed_flag; break;
                default: break;
            }
        }
    }

    auto en_passant_sq_FEN = sections[3];
    if(en_passant_sq_FEN == "-"){
        en_passant_sq = 0;
    }
    else{
        std::from_chars(en_passant_sq_FEN.data(), en_passant_sq_FEN.data() + en_passant_sq_FEN.size(), en_passant_sq);
    }

    auto clock_FEN = sections[4];
    std::from_chars(clock_FEN.data(), clock_FEN.data() + clock_FEN.size(), clock);

    auto num_moves_FEN = sections[5];
    std::from_chars(num_moves_FEN.data(), num_moves_FEN.data() + num_moves_FEN.size(), num_moves);

    return {};
}


void Board::reset(){
    using namespace tables::defaults;
    bitboards = bitboards_init;
    all_pieces = all_pieces_init;
    pieces = pieces_init;
    en_passant_sq = 0;
    castle_rights = white_left_castle_allowed_flag | white_right_castle_allowed_flag | black_left_castle_allowed_flag | black_right_castle_allowed_flag;
    clock = 0;
    turn = Color::WHITE;
    pinned = 0;
    num_moves = 0; //currently unused
    checkers = 0;
}
