#ifndef BOARD_INL
#define BOARD_INL
#include "piece.hpp"
#include "bitboard_moves.hpp"
#include "board.hpp"



template<Color attacker_color>
std::uint64_t Board::get_attackers(int sq){
    using namespace bitboard_moves;
    using enum PieceType;
    constexpr Color defender_color = other_color<attacker_color>();
    std::uint64_t allies = all_pieces[defender_color];
    std::uint64_t enemies = all_pieces[attacker_color];
    std::uint64_t enemy_queen = bitboards[to_piece(attacker_color, PieceType::QUEEN)];
    return 
        (knight(sq, allies, enemies) & bitboards[to_piece(attacker_color, KNIGHT)]) | 
        (bishop(sq, allies, enemies) & (bitboards[to_piece(attacker_color, BISHOP)] | enemy_queen)) | 
        (rook(sq, allies, enemies) & (bitboards[to_piece(attacker_color, ROOK)] | enemy_queen)) | 
        (pawn_captures<defender_color>(sq, enemies) & bitboards[to_piece(attacker_color, PAWN)]) |
        (king(sq, allies, enemies) & bitboards[to_piece(attacker_color, KING)]);
}


template<Color attacker_color>
bool Board::is_attacked(int sq){
    using namespace bitboard_moves;
    using enum PieceType;
    constexpr Color defender_color = other_color(attacker_color);
    std::uint64_t allies = all_pieces[defender_color];
    std::uint64_t enemies = all_pieces[attacker_color];
    std::uint64_t enemy_queen = bitboards[to_piece(attacker_color, QUEEN)];
    if(rook(sq, allies, enemies) & (bitboards[to_piece(attacker_color, ROOK)] | enemy_queen)){
        return true;
    } 
    else if(bishop(sq, allies, enemies) & (bitboards[to_piece(attacker_color, BISHOP)] | enemy_queen)){
        return true;
    }
    else if(knight(sq, allies, enemies) & bitboards[to_piece(attacker_color, KNIGHT)]){
        return true;
    }
    else if(pawn_captures<defender_color>(sq, enemies) & bitboards[to_piece(attacker_color, PAWN)]){
        return true;
    }
    else if(king(sq, allies, enemies) & bitboards[to_piece(attacker_color, KING)]){
        return true;
    }
    return false;
}


template<Color c>
int Board::get_king_pos(){
    return std::countr_zero(bitboards[to_piece(c, PieceType::KING)]);
}


template<Color c>
bool Board::in_check(){
    return is_attacked<other_color<c>()>(get_king_pos<c>());
}


template<Color c>
std::uint64_t Board::get_checkers(){
    return get_attackers<other_color<c>()>(get_king_pos<c>());
}


template<Color c>
std::uint64_t Board::get_pawn_starting_row(){
    return tables::defaults::bitboards_init[to_piece(c, PieceType::PAWN)];
}


template<Color c>
std::uint64_t Board::get_pawn_double_push_row(){
    using namespace bitboard;
    return c == Color::WHITE ? slide<N>(slide<N>(get_pawn_starting_row<c>())) :
                                slide<S>(slide<S>(get_pawn_starting_row<c>()));
}


template<Color c>
std::uint64_t Board::get_en_passant_row(){
    return c == Color::WHITE ? get_pawn_double_push_row<Color::BLACK>() : get_pawn_double_push_row<Color::WHITE>();
}


template<Color c>
std::uint64_t Board::get_promotion_row(){
    return c == Color::WHITE ? 255ull << 56: 255ull;
}


template<Color c>
void Board::recalculate_pinned(){
    using namespace tables::pins;
    using enum PieceType;
    constexpr Color enemy_color = other_color(c);
    int king_sq = get_king_pos<c>();
    pinned = 0;
    std::uint64_t enemy_queens = bitboards[to_piece(enemy_color, QUEEN)];
    std::uint64_t diagonal_attackers = (bitboards[to_piece(enemy_color, BISHOP)] | enemy_queens) & diagonal_mask[king_sq];
    std::uint64_t straight_attackers = (bitboards[to_piece(enemy_color, ROOK)] | enemy_queens) & straight_mask[king_sq];
    std::uint64_t all_occ = all_pieces[Color::WHITE] | all_pieces[Color::BLACK];
    while(diagonal_attackers){
        int pinner_sq = std::countr_zero(diagonal_attackers);
        assert(pinner_sq >= 0 && pinner_sq < 64);
        std::uint64_t blockers = diagonal_between[king_sq][pinner_sq] & all_occ;
        std::uint64_t ally_blockers = blockers & all_pieces[c];
        if(std::has_single_bit(blockers) && (ally_blockers != 0)){
            pinned |= ally_blockers;
        }
        diagonal_attackers ^= (1ull << pinner_sq);
    }
    while(straight_attackers){
        int pinner_sq = std::countr_zero(straight_attackers);
        std::uint64_t blockers = straight_between[king_sq][pinner_sq] & all_occ;
        std::uint64_t ally_blockers = blockers & all_pieces[c];
        if(std::has_single_bit(blockers) && (ally_blockers != 0)){
            pinned |= ally_blockers;
        }
        straight_attackers ^= (1ull << pinner_sq);
    }
}


template<Color c>
void Board::do_castle(int src, int dest){
    assert(c == get_color(pieces[src]));
    constexpr Piece rook = to_piece(c, PieceType::ROOK);
    constexpr Piece king = to_piece(c, PieceType::KING);
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
    castle_rights &= ~(c == Color::WHITE ? (white_left_castle_allowed_flag | white_right_castle_allowed_flag) : 
                                                     (black_left_castle_allowed_flag | black_right_castle_allowed_flag));
}


template<Color c>
void Board::move_piece(int src, int dest){
    Piece attacker = pieces[src];
    Piece captured = pieces[dest];
    assert(c == get_color(attacker));
    std::uint64_t dest_mask = 1ull << dest;
    std::uint64_t move_mask = (1ull << src) | dest_mask;
    bitboards[attacker] ^= move_mask;
    pieces[src] = Piece::EMPTY;
    if(captured != Piece::EMPTY){
        bitboards[captured] ^= dest_mask;
    }
    pieces[dest] = attacker;
    PieceType attacker_type = get_type(attacker);
    if(attacker_type == PieceType::KING){
        castle_rights &= ~(c == Color::WHITE ? (white_left_castle_allowed_flag | white_right_castle_allowed_flag) : 
                                                     (black_left_castle_allowed_flag | black_right_castle_allowed_flag));
    }
    else if(src == 0 || dest == 0) castle_rights &= ~(white_right_castle_allowed_flag);
    else if(src == 7 || dest == 7) castle_rights &= ~(white_left_castle_allowed_flag);
    else if(src == 56 || dest == 56) castle_rights &= ~(black_right_castle_allowed_flag);
    else if(src == 63 || dest == 63) castle_rights &= ~(black_left_castle_allowed_flag);
}


template<Color c>
void Board::promote(int sq, PieceType p){
    assert(c == get_color(pieces[sq]));
    std::uint64_t pos = 1ull << sq;
    constexpr Piece pawn = to_piece(c, PieceType::PAWN);
    Piece promoted_piece = to_piece(c, p);
    bitboards[pawn] ^= pos;
    bitboards[promoted_piece] ^= pos;
    pieces[sq] = promoted_piece;
}


template<Color c>
void Board::do_en_passant(int en_passant_sq){
    assert(c == get_color(pieces[en_passant_sq]));
    constexpr Color enemy_color = other_color(c);
    constexpr Piece pawn = to_piece(enemy_color, PieceType::PAWN);
    int captured_piece_sq = (c == Color::WHITE ? en_passant_sq - 8 : en_passant_sq + 8);
    std::uint64_t captured_mask = 1ull << captured_piece_sq;
    bitboards[pawn] ^= captured_mask;
    pieces[captured_piece_sq] = Piece::EMPTY;
    en_passant_sq = 0;
}


template<Color c>
void Board::make_move(Move move){
    constexpr Color enemy_color = other_color<c>();
    int src = move.src();
    int dest = move.dest();
    Piece moved = pieces[src];
    prev_moves[cur_ply] = History(move, UndoMove(en_passant_sq, pieces[dest], castle_rights, clock, pinned, checkers));
    cur_ply++;
    if(cur_ply % 2 == 0) num_moves++;
    if(move.is_castle()){
        do_castle<c>(src, dest);
    }
    else{
        move_piece<c>(src, dest);
        if(move.is_promotion()){
            promote<c>(dest, move.promoted_type());
        }
        else if(move.is_en_passant()){
            do_en_passant<c>(en_passant_sq);
        }
    }
    if(get_type(moved) == PieceType::PAWN){
        clock = 0;
        if(((1ull << src) & get_pawn_starting_row<c>()) && ((1ull << dest) & get_pawn_double_push_row<c>())){
            en_passant_sq = (c == Color::WHITE) ? dest - 8 : dest + 8;
        }
        else en_passant_sq = 0;
    }
    else{
        en_passant_sq = 0;
        if(move.is_capture()) clock = 0;
        else clock++;
    }
    turn = enemy_color;
    recalculate_all_pieces();
    recalculate_pinned<enemy_color>();
    checkers = get_checkers<enemy_color>();
    assert_valid();
    //Must omit the following line in debug builds, since en passant move gen relies on checking if move leaves ally king in check to determine legality
    //assert(!in_check(other_color(enemy_color)) && "Cannot leave ally king in check"); 
}


template<Color c>
void Board::undo_promote(int sq){
    Piece promoted_piece = pieces[sq];
    assert(c == get_color(promoted_piece));
    constexpr Piece pawn = to_piece(c, PieceType::PAWN);
    std::uint64_t pos = 1ull << sq;
    bitboards[pawn] ^= pos;
    bitboards[promoted_piece] ^= pos;
    pieces[sq] = pawn;
}


template<Color c>
void Board::undo_en_passant(int en_passant_sq){
    constexpr Color captured_color = other_color<c>();
    constexpr Piece captured_piece = to_piece(captured_color, PieceType::PAWN);
    int captured_piece_sq = c == Color::WHITE ? en_passant_sq - 8 : en_passant_sq + 8;
    std::uint64_t captured_mask = (1ull << captured_piece_sq);
    bitboards[captured_piece] ^= captured_mask;
    pieces[captured_piece_sq] = captured_piece;
}


template<Color c>
void Board::undo_move_piece(int src, int dest, Piece captured_piece){
    Piece attacker = pieces[dest];
    assert(c == get_color(attacker));
    std::uint64_t dest_mask = 1ull << dest;
    std::uint64_t move_mask = (1ull << src) | dest_mask;
    bitboards[attacker] ^= move_mask;
    pieces[src] = pieces[dest];
    pieces[dest] = captured_piece;
    if(captured_piece != Piece::EMPTY){
        bitboards[captured_piece] |= dest_mask;
    }
}


template<Color c>
void Board::undo_castle(int src, int dest){
    constexpr Piece king = to_piece(c, PieceType::KING);
    constexpr Piece rook = to_piece(c, PieceType::ROOK);
    if(src < dest){
        pieces[src] = pieces[src + 2];
        pieces[dest] = pieces[src + 1];
        bitboards[king] >>= 2;
        bitboards[rook] ^= ((1ull << dest) | (1ull << (src + 1)));
        pieces[src + 2] = Piece::EMPTY;
        pieces[src + 1] = Piece::EMPTY;
    }
    else{
        pieces[src] = pieces[src - 2];
        pieces[dest] = pieces[src - 1];
        bitboards[king] <<= 2;
        bitboards[rook] ^= ((1ull << dest) | (1ull << (src - 1)));
        pieces[src - 2] = Piece::EMPTY;
        pieces[src - 1] = Piece::EMPTY;
    }
}


template<Color c>
void Board::undo_last_move(){
    if(cur_ply % 2 == 0) num_moves--;
    cur_ply--;
    auto& [move, undo_info] = prev_moves[cur_ply];
    int src = move.src();
    int dest = move.dest();
    turn = c;
    if(move.is_castle()){
        undo_castle<c>(src, dest);
    }
    else{
        if(move.is_promotion()){
            undo_promote<c>(dest);
        }
        undo_move_piece<c>(src, dest, undo_info.captured_piece());
        if(move.is_en_passant()){
            undo_en_passant<c>(undo_info.en_passant_sq());
        }
    }
    en_passant_sq = undo_info.en_passant_sq();
    castle_rights = undo_info.castle_rights();
    pinned = undo_info.prev_enemy_pinned();
    checkers = undo_info.prev_checkers();
    clock = undo_info.clock();
    recalculate_all_pieces();
    assert_valid();
}


template<Color c>
std::uint64_t Board::get_legal_castle_moves(){
    if(in_check<c>()) return 0;
    std::uint64_t moves = 0;
    std::uint64_t occ = all_pieces[Color::WHITE] | all_pieces[Color::BLACK];
    constexpr Color enemy_color = other_color<c>();
    if constexpr(c == Color::WHITE){
        constexpr std::uint64_t white_left_castle_mask = (1ull << 4) | (1ull << 5) | (1ull << 6);
        constexpr std::uint64_t white_right_castle_mask = (1ull << 1) | (1ull << 2);
        if((castle_rights & white_left_castle_allowed_flag) && !(occ & white_left_castle_mask) && !is_attacked<enemy_color>(4) && !is_attacked<enemy_color>(5)){
            moves |= (1ull << 7);
        }
        if((castle_rights & white_right_castle_allowed_flag) && !(occ & white_right_castle_mask) && !is_attacked<enemy_color>(2) && !is_attacked<enemy_color>(1)){
            moves |= 1;
        }
    }
    else{
        constexpr std::uint64_t black_left_castle_mask = (1ull << 60) | (1ull << 61) | (1ull << 62);
        constexpr std::uint64_t black_right_castle_mask = (1ull << 57) | (1ull << 58);
        if((castle_rights & black_left_castle_allowed_flag) && !(occ & black_left_castle_mask) && !is_attacked<enemy_color>(60) && !is_attacked<enemy_color>(61)){
            moves |= (1ull << 63);
        }
        if((castle_rights & black_right_castle_allowed_flag) && !(occ & black_right_castle_mask) && !is_attacked<enemy_color>(57) && !is_attacked<enemy_color>(58)){
            moves |= (1ull << 56);
        }
    }
    return moves;
}


template<Color c>
std::uint64_t Board::get_quiets_and_captures(int sq){
    Piece piece = pieces[sq];
    assert(c == get_color(piece));
    std::uint64_t allies = all_pieces[c];
    std::uint64_t enemies = all_pieces[other_color<c>()];
    switch(get_type(piece)){
        case PieceType::ROOK: return bitboard_moves::rook(sq, allies, enemies);
        case PieceType::BISHOP: return bitboard_moves::bishop(sq, allies, enemies);
        case PieceType::QUEEN: return bitboard_moves::queen(sq, allies, enemies);
        case PieceType::KNIGHT: return bitboard_moves::knight(sq, allies, enemies);
        case PieceType::KING: return bitboard_moves::king(sq, allies, enemies);
        case PieceType::PAWN: return bitboard_moves::pawn<c>(sq, allies, enemies);
        case PieceType::EMPTY: return 0;
    }
    return 0;
}


//An exception will occur at get_checkers if the user tries to move a piece that is already attacking the opponent king. 
//This is because when testing all possible moves that piece may make for legality, 
//one of those moves will involve capturing the opponent king. 
//It is not possible to determine if the opponent king is in check, if it doesn't exist on the board!
//With turns-based moves, this exception will never occur
template<Color c>
std::uint64_t Board::get_legal_quiets_and_captures(int sq){
    Piece piece = pieces[sq];
    assert(c == get_color(piece));
    constexpr Color enemy_color = other_color<c>();
    PieceType ally_type = get_type(piece);
    //cannot block a double check with a non-king piece
    if((std::popcount(checkers) == 2 && ally_type != PieceType::KING)){
        return 0;
    }
    std::uint64_t attacks = get_quiets_and_captures<c>(sq);
    if(ally_type == PieceType::KING){
        std::uint64_t legal_moves = 0;
        bitboards[piece] = 0;
        all_pieces[c] ^= (1ull << sq);
        while(attacks){
            int dest = std::countr_zero(attacks);
            std::uint64_t dest_mask = 1ull << dest;
            if(!is_attacked<enemy_color>(dest)){
                legal_moves |= dest_mask;
            }
            attacks ^= dest_mask;
        }
        bitboards[piece] = 1ull << sq;
        all_pieces[c] ^= (1ull << sq);
        return legal_moves;
    }
    else{
        //if this point reached, piece is not king and there is at most one checker
        using namespace tables::pins;
        bool is_checked = (checkers != 0);
        int king_sq = get_king_pos<c>();
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


template<Color c>
std::uint64_t Board::get_legal_en_passant_moves(int sq){
    Piece pawn = pieces[sq];
    assert(c == get_color(pawn));
    if(get_type(pawn) != PieceType::PAWN || ((1ull << sq) & get_en_passant_row<c>()) == 0){
        return 0;
    }
    std::uint64_t move = bitboard_moves::pawn_en_passant<c>(sq, en_passant_sq);
    if(move == 0){
        return 0;
    }
    make_move<c>(Move(sq, std::countr_zero(move), Move::en_passant_flag | Move::capture_flag));
    bool results_in_check = in_check<c>();
    undo_last_move<c>();
    return results_in_check ? 0 : move;
}


template<Color c>
void Board::populate_legal_moves(int sq, MoveList& list){
    Piece piece = pieces[sq];
    assert(piece != Piece::EMPTY && c == get_color(piece) && "Handle empty piece case in the dispatch for this templated function");
    if(c != turn){
        return;
    }
    PieceType ally_type = get_type(piece);
    constexpr Color enemy_color = other_color<c>();
    std::uint64_t quiets_and_captures = get_legal_quiets_and_captures<c>(sq);
    std::uint64_t enemies = all_pieces[enemy_color];
    std::uint64_t en_passant = get_legal_en_passant_moves<c>(sq);
    std::uint64_t castles = ally_type == PieceType::KING ? get_legal_castle_moves<c>() : 0;
    // keep in mind for future: if a pawn has one promotion move it can currently perform, then all of its moves must be promotions also
    while(quiets_and_captures){
        int dest = std::countr_zero(quiets_and_captures);
        std::uint64_t dest_mask = 1ull << dest;
        Move::MoveFlag flags = Move::empty_flag;
        if(dest_mask & enemies){
            flags |= Move::capture_flag;
        }
        if((dest_mask & get_promotion_row<c>()) && ally_type == PieceType::PAWN){
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
#endif