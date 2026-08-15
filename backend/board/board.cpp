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
#include <climits>
#include <algorithm>
#include "score_weights.hpp"


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


std::size_t Board::get_cur_ply(){
    return cur_ply;
}

//WILL throw an exception if the move stack is currently full (i.e. cur_ply > MAX_PLYS)
void Board::make_move(Move move){
    return turn == Color::WHITE ? make_move<Color::WHITE>(move) : make_move<Color::BLACK>(move);
}


void Board::undo_last_move(){
    return turn == Color::WHITE ? undo_last_move<Color::BLACK>() : undo_last_move<Color::WHITE>();
}


void Board::recalculate_all_pieces(){
    all_pieces[Color::WHITE] = bitboards[Piece::WHITE_PAWN] | bitboards[Piece::WHITE_ROOK] | bitboards[Piece::WHITE_KNIGHT] | bitboards[Piece::WHITE_BISHOP] | bitboards[Piece::WHITE_QUEEN] | bitboards[Piece::WHITE_KING];
    all_pieces[Color::BLACK] = bitboards[Piece::BLACK_PAWN] | bitboards[Piece::BLACK_ROOK] | bitboards[Piece::BLACK_KNIGHT] | bitboards[Piece::BLACK_BISHOP] | bitboards[Piece::BLACK_QUEEN] | bitboards[Piece::BLACK_KING];
}



void Board::recalculate_pinned(Color c){
    if(c == Color::WHITE){
        recalculate_pinned<Color::WHITE>();
    }
    else{
        recalculate_pinned<Color::BLACK>();
    }
}


BoardState Board::get_board_state(){
    return BoardState(*this);
}


int Board::get_king_pos(Color c){
    return c == Color::WHITE ? get_king_pos<Color::WHITE>() : get_king_pos<Color::BLACK>();
}


bool Board::turn_color_in_check(){
    return checkers != 0;
}


bool Board::turn_color_in_checkmate(){
    if(turn_color_in_check()){
        MoveList list;
        populate_legal_moves(list);
        if(list.size() == 0){
            return true;
        }
    }
    return false;
}


void Board::populate_legal_moves(int sq, MoveList& list){
    Piece piece = pieces[sq];
    if(piece == Piece::EMPTY) return;
    if(get_color(piece) == Color::WHITE){
        populate_legal_moves<Color::WHITE>(sq, list);
    }
    else{
        populate_legal_moves<Color::BLACK>(sq, list);
    }
}


void Board::populate_legal_moves(MoveList& list){
    for(int sq = 0; sq < 64; sq++){
        populate_legal_moves(sq, list);
    }
}


std::string Board::layout(){
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


void Board::perft_helper(int depth, PerftResults& stats){
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
            perft_helper(depth - 1, stats);
        }
        undo_last_move();
    }
}


PerftResults Board::perft(int depth){
    PerftResults stats;
    if(depth == 0){
        stats.nodes++;
        return stats;
    }
    perft_helper(depth, stats);
    return stats;
}


std::uint64_t Board::fast_perft_helper(int depth){
    MoveList move_list;
    populate_legal_moves(move_list);
    if(depth == 1) return move_list.size();
    std::uint64_t cnt = 0;
    for(int i = 0; i < move_list.size(); i++){
        Move move = move_list[i];
        make_move(move);
        cnt += fast_perft_helper(depth - 1);
        undo_last_move();
    }
    return cnt;
}


std::uint64_t Board::fast_perft(int depth){
    return depth == 0 ? 1 : fast_perft_helper(depth);
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
    
    cur_ply = 0;
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


int Board::score(){
    using enum Piece;
    std::uint64_t white_rooks = bitboards[WHITE_ROOK];
    std::uint64_t black_rooks = bitboards[BLACK_ROOK];
    std::uint64_t white_bishops = bitboards[WHITE_BISHOP];
    std::uint64_t black_bishops = bitboards[BLACK_BISHOP];
    std::uint64_t white_queens = bitboards[WHITE_QUEEN];
    std::uint64_t black_queens = bitboards[BLACK_QUEEN];
    std::uint64_t white_knights = bitboards[WHITE_KNIGHT];
    std::uint64_t black_knights = bitboards[BLACK_KNIGHT];
    int material_score = 
        material_weights::pawn_weight * (std::popcount(bitboards[WHITE_PAWN]) - std::popcount(bitboards[BLACK_PAWN])) + 
        material_weights::rook_weight * (std::popcount(white_rooks) - std::popcount(black_rooks)) + 
        material_weights::knight_weight * (std::popcount(white_knights) - std::popcount(black_knights)) + 
        material_weights::bishop_weight * (std::popcount(white_bishops) - std::popcount(black_bishops)) + 
        material_weights::queen_weight * (std::popcount(white_queens) - std::popcount(black_queens));
    auto mobility_diff = [&](std::uint64_t white_sqs, std::uint64_t black_sqs){
        int diff = 0;
        while(white_sqs){
            int white_sq = std::countr_zero(white_sqs);
            diff += std::popcount(get_quiets_and_captures<Color::WHITE>(white_sq));
            white_sqs ^= (1ull << white_sq);
        }
        while(black_sqs){
            int black_sq = std::countr_zero(black_sqs);
            diff -= std::popcount(get_quiets_and_captures<Color::BLACK>(black_sq));
            black_sqs ^= (1ull << black_sq);
        }
        return diff;
    };
    int mobility_score = 
        mobility_weights::knight_weight * mobility_diff(white_knights, black_knights) + 
        mobility_weights::bishop_weight * mobility_diff(white_bishops, black_bishops) + 
        mobility_weights::rook_weight * mobility_diff(white_rooks, black_rooks) + 
        mobility_weights::queen_weight * mobility_diff(white_queens, black_queens);
    return (material_score + mobility_score) * (turn == Color::WHITE ? 1 : -1);
}


int Board::evaluate_move(Move move){
    int sc = 10;
    Piece capturer = pieces[move.src()];
    Piece captured = pieces[move.dest()];
    if(move.is_capture()){
        sc += 10 * get_weight(get_type(captured)) - get_weight(get_type(capturer)) + 60; 
    }
    if(move.is_promotion()){
        sc += 40 + 3 * (get_weight(move.promoted_type()));
    }
    else if(move.is_castle()){
        sc += 10;
    }
    return sc;
}


//returns the best move searching until ply "depth"
int Board::best_score(int depth, int a, int b){
    if(turn_color_in_checkmate()){
        return turn == Color::WHITE ? INT_MAX : INT_MIN;
    }
    if(depth == 0){
        return score();
    }
    MoveList list;
    populate_legal_moves(list);
    int best_board_score = INT_MIN;
    for(int start = 0; start < list.size(); start++){
        int cur_best_move_score = INT_MIN;
        int cur_best_move_index;
        for(int i = start; i < list.size(); i++){
            Move move = list[i];
            int move_score = evaluate_move(move);
            if(move_score > cur_best_move_score){
                cur_best_move_score = move_score;
                cur_best_move_index = i;
            }
        }

        make_move(list[cur_best_move_index]);
        int tree_score = best_score(depth - 1, a, b);
        undo_last_move();
        if(turn == Color::WHITE){
            tree_score -= depth;
        }
        else{
            tree_score += depth;
        }
        best_board_score = std::max(best_board_score, tree_score);
        if((turn == Color::WHITE && b <= tree_score) || (turn == Color::BLACK && a >= tree_score)){
            break;
        }
        a = std::max(a, tree_score);
        b = std::min(b, tree_score);

        Move tmp = list[start];
        list.set(start, list[cur_best_move_index]);
        list.set(cur_best_move_index, tmp);
    }
    return best_board_score;
}


std::tuple<Move, int> Board::ids(int depth){
    assert(depth > 0 && !turn_color_in_checkmate());
    Move best_move;
    int best_board_score = INT_MIN;
    MoveList list;
    populate_legal_moves(list);
    while(depth){
        int a = INT_MIN;
        int b = INT_MAX;
        Move best_depth_move;
        int best_depth_board_score = INT_MIN;
        for(int start = 0; start < list.size(); start++){
            int cur_best_move_score = INT_MIN;
            int cur_best_move_index;
            for(int i = start; i < list.size(); i++){
                Move move = list[i];
                int move_score = evaluate_move(move);
                if(move_score > cur_best_move_score){
                    cur_best_move_score = move_score;
                    cur_best_move_index = i;
                }
            }
            make_move(list[cur_best_move_index]);
            int tree_score = best_score(depth - 1, a, b);
            undo_last_move();
            if(turn == Color::WHITE){
                tree_score -= depth;
            }
            else{
                tree_score += depth;
            }
            if(tree_score > best_board_score){
                best_board_score = tree_score;
                best_move = list[cur_best_move_index];
            }
            if((turn == Color::WHITE && b <= tree_score) || (turn == Color::BLACK && a >= tree_score)){
                break;
            }
            a = std::max(a, tree_score);
            b = std::min(b, tree_score);

            Move tmp = list[start];
            list.set(start, list[cur_best_move_index]);
            list.set(cur_best_move_index, tmp);
        }
        depth--;
    }
    return std::make_tuple(best_move, best_board_score);
}


//result is undefined if the current board state is a checkmate
Move Board::best_move(){
    if(turn_color_in_checkmate()){
        return Move();
    }
    return std::get<0>(ids(MAX_SEARCH_DEPTH));
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
    num_moves = 1; //currently unused
    cur_ply = 0;
    checkers = 0;
}
