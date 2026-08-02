#include "board.hpp"
#include "move.hpp"
#include "bitboard_moves.hpp"
#include "undo_move.hpp"
#include "perft_results.hpp"
#include <bit>
#include <utility>
#include <cassert>


namespace{
    inline constexpr std::uint64_t white_right_all_mask = 15;
    inline constexpr std::uint64_t white_left_all_mask = (1ull << 3) | (1ull << 4) | (1ull << 5) | (1ull << 7);
    inline constexpr std::uint64_t black_right_all_mask = (1ull << 56) | (1ull << 57) | (1ull << 58) | (1ull << 59);
    inline constexpr std::uint64_t black_left_all_mask = (1ull << 59) | (1ull << 60) | (1ull << 61) | (1ull << 63);
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
        pieces[src + 2] = pieces[src];
        pieces[src + 1] = pieces[dest];
        all_pieces[turn] ^= (turn == Color::WHITE ? white_left_all_mask : black_left_all_mask);
    }
    else{
        bitboards[king] >>= 2;
        bitboards[rook] ^= ((1ull << dest) | (1ull << (src - 1)));
        pieces[src - 2] = pieces[src];
        pieces[src - 1] = pieces[dest];
        all_pieces[turn] ^= (turn == Color::WHITE ? white_right_all_mask : black_right_all_mask);
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
    if(get_type(attacker) == PieceType::ROOK){
        if(src > get_king_pos(turn)){
            castle_rights &= ~(turn == Color::WHITE ? white_left_castle_allowed_flag : black_left_castle_allowed_flag);
        }
        else{
            castle_rights &= ~(turn == Color::WHITE ? white_right_castle_allowed_flag : black_right_castle_allowed_flag);
        }
    }
    else if(get_type(attacker) == PieceType::KING){
        castle_rights &= ~(turn == Color::WHITE ? (white_left_castle_allowed_flag | white_right_castle_allowed_flag) : 
                                                     (black_left_castle_allowed_flag | black_right_castle_allowed_flag));
    }
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
}


void Board::make_move(Move move){
    int src = move.src();
    int dest = move.dest();
    Piece moved = pieces[src];
    Color enemy_color = other_color(turn);
    prev_moves.emplace(move, UndoMove(en_passant_sq, get_type(pieces[dest]), castle_rights, clock, pinned));
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
    if(moved == Piece::WHITE_PAWN){
        clock = 0;
        if(dest - src == 16){
            en_passant_sq = src + 8;
        }
    }
    else if(moved == Piece::BLACK_PAWN){
        clock = 0;
        if(src - dest == 16){
            en_passant_sq = dest + 8;
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
    recalculate_pinned(enemy_color);
    assert_valid();
}


void Board::recalculate_all_pieces(){
    all_pieces[Color::WHITE] = bitboards[Piece::WHITE_PAWN] | bitboards[Piece::WHITE_ROOK] | bitboards[Piece::WHITE_KNIGHT] | bitboards[Piece::WHITE_BISHOP] | bitboards[Piece::WHITE_QUEEN] | bitboards[Piece::WHITE_KING];
    all_pieces[Color::BLACK] = bitboards[Piece::BLACK_PAWN] | bitboards[Piece::BLACK_ROOK] | bitboards[Piece::BLACK_KNIGHT] | bitboards[Piece::BLACK_BISHOP] | bitboards[Piece::BLACK_QUEEN] | bitboards[Piece::BLACK_KING];
}


std::uint64_t Board::get_attackers(int sq) const{
    Color enemy_color = other_color(turn);
    std::uint64_t allies = all_pieces[turn];
    std::uint64_t enemies = all_pieces[enemy_color];
    std::uint64_t enemy_queen = bitboards[to_piece(enemy_color, PieceType::QUEEN)];
    return 
        (bitboard_moves::knight(sq, allies, enemies) & bitboards[to_piece(enemy_color, PieceType::KNIGHT)]) | 
        (bitboard_moves::bishop(sq, allies, enemies) & (bitboards[to_piece(enemy_color, PieceType::BISHOP)] | enemy_queen)) | 
        (bitboard_moves::rook(sq, allies, enemies) & (bitboards[to_piece(enemy_color, PieceType::ROOK)] | enemy_queen)) | 
        (bitboard_moves::pawn_captures(sq, turn, enemies) & bitboards[to_piece(enemy_color, PieceType::PAWN)]) |
        (bitboard_moves::king(sq, allies, enemies) & bitboards[to_piece(enemy_color, PieceType::KING)]);
}


bool Board::is_attacked(int sq) const{
    Color enemy_color = other_color(turn);
    std::uint64_t allies = all_pieces[turn];
    std::uint64_t enemies = all_pieces[enemy_color];
    std::uint64_t enemy_queen = bitboards[to_piece(enemy_color, PieceType::QUEEN)];
    if(bitboard_moves::rook(sq, allies, enemies) & (bitboards[to_piece(enemy_color, PieceType::ROOK)] | enemy_queen)){
        return true;
    } 
    else if(bitboard_moves::bishop(sq, allies, enemies) & (bitboards[to_piece(enemy_color, PieceType::BISHOP)] | enemy_queen)){
        return true;
    }
    else if(bitboard_moves::knight(sq, allies, enemies) & bitboards[to_piece(enemy_color, PieceType::KNIGHT)]){
        return true;
    }
    else if(bitboard_moves::pawn_captures(sq, turn, enemies) & bitboards[to_piece(enemy_color, PieceType::PAWN)]){
        return true;
    }
    else if(bitboard_moves::king(sq, allies, enemies) & bitboards[to_piece(enemy_color, PieceType::KING)]){
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
    clock = undo_info.clock();
    recalculate_all_pieces();
    assert_valid();
}


std::uint64_t Board::get_checkers(Color c) const{
    return get_attackers(get_king_pos(c));
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
    std::uint64_t checkers = get_checkers(ally_color);
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
            if(!is_attacked(dest)){
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


std::uint64_t Board::get_castle_moves(Color c){
    std::uint64_t moves = 0;
    if(in_check(c)) return moves;
    std::uint64_t occ = all_pieces[Color::WHITE] | all_pieces[Color::BLACK];
    if(c == Color::WHITE){
        constexpr std::uint64_t white_left_castle_mask = (1ull << 4) | (1ull << 5) | (1ull << 6);
        constexpr std::uint64_t white_right_castle_mask = (1ull << 1) | (1ull << 2);
        if((castle_rights & white_left_castle_allowed_flag) && !(occ & white_left_castle_mask) && !is_attacked(4) && !is_attacked(5)){
            moves |= (1ull << 7);
        }
        if((castle_rights & white_left_castle_allowed_flag) && !(occ & white_right_castle_mask) && !is_attacked(2) && !is_attacked(1)){
            moves |= 1;
        }
    }
    else{
        constexpr std::uint64_t black_left_castle_mask = (1ull << 60) | (1ull << 61) | (1ull << 62);
        constexpr std::uint64_t black_right_castle_mask = (1ull << 57) | (1ull << 58);
        if((castle_rights & black_left_castle_allowed_flag) && !(occ & black_left_castle_mask) && !is_attacked(60) && !is_attacked(61)){
            moves |= (1ull << 63);
        }
        if((castle_rights & black_right_castle_allowed_flag) && !(occ & black_right_castle_mask) && !is_attacked(57) && !is_attacked(58)){
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
    Color enemy_color = other_color(ally_color);
    std::uint64_t quiets_and_captures = get_legal_quiets_and_captures(sq);
    std::uint64_t enemies = all_pieces[enemy_color];
    std::uint64_t en_passant = (get_type(piece) == PieceType::PAWN && ((1ull << sq) & get_en_passant_row(ally_color))) ? bitboard_moves::pawn_en_passant(sq, ally_color, en_passant_sq) : 0;
    std::uint64_t castles = ally_type == PieceType::KING ? get_castle_moves(ally_color) : 0;
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
            legal_moves.add_move(Move(sq, dest, flags, PieceType::QUEEN));
            legal_moves.add_move(Move(sq, dest, flags, PieceType::KNIGHT));
            legal_moves.add_move(Move(sq, dest, flags, PieceType::BISHOP));
            legal_moves.add_move(Move(sq, dest, flags, PieceType::ROOK));
        }
        else{
            legal_moves.add_move(Move(sq, dest, flags));
        }
        quiets_and_captures ^= (1ull << dest);
    }
    if(en_passant){
        legal_moves.add_move(Move(sq, en_passant_sq, Move::en_passant_flag | Move::capture_flag));
    }
    while(castles){
        int dest = std::countr_zero(castles);
        legal_moves.add_move(Move(sq, dest, Move::castle_flag));
        castles ^= (1ull << dest);
    }
    return legal_moves;
}


bool Board::in_check(Color c){
    return is_attacked(get_king_pos(c));
}


bool Board::turn_color_in_check(){
    return in_check(turn);
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


void Board::assert_valid(){
    constexpr std::array<Piece, 12> piece_enum = {
        Piece::WHITE_ROOK, Piece::WHITE_KNIGHT, Piece::WHITE_BISHOP, Piece::WHITE_QUEEN, Piece::WHITE_KING, Piece::WHITE_PAWN, 
        Piece::BLACK_PAWN, Piece::BLACK_ROOK, Piece::BLACK_KNIGHT, Piece::BLACK_BISHOP, Piece::BLACK_QUEEN, Piece::BLACK_KING
    };
    std::uint64_t white_pieces = all_pieces[Color::WHITE];
    std::uint64_t black_pieces = all_pieces[Color::BLACK];
    recalculate_all_pieces();
    assert(white_pieces == all_pieces[Color::WHITE] && black_pieces == all_pieces[Color::BLACK]);
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
    for(int sq = 0; sq < 64; sq++){
        MoveList move_list = get_legal_moves(sq);
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
}
