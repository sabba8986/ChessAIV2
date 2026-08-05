#ifndef BOARD
#define BOARD

#include "piece.hpp"
#include "move.hpp"
#include "move_list.hpp"
#include "enum_arr.hpp"
#include "history.hpp"
#include <cstdint>
#include <array>
#include <stack>
#include <bit>
#include "board_state.hpp"
#include "perft_results.hpp"
#include "pch.hpp"
#include <string>
#include "fen_error.hpp"
#include <expected>

class Board{
    static constexpr std::uint8_t white_left_castle_allowed_flag = 1;
    static constexpr std::uint8_t white_right_castle_allowed_flag = 2;
    static constexpr std::uint8_t black_left_castle_allowed_flag = 4;
    static constexpr std::uint8_t black_right_castle_allowed_flag = 8;
    
    EnumArr<std::uint64_t, 15> bitboards;
    EnumArr<std::uint64_t, 2> all_pieces;
    EnumArr<Piece, 64> pieces;
    std::uint64_t pinned;
    std::uint64_t checkers;
    std::stack<History> prev_moves;
    int en_passant_sq;
    std::uint8_t castle_rights;
    int clock;
    Color turn;
    int num_moves;

    std::uint64_t get_checkers(Color c) const;

    void do_castle(int src, int dest);
    void move_piece(int src, int dest);
    void promote(int sq, PieceType p);
    void do_en_passant(int en_passant_sq);
    void undo_castle(int src, int dest);
    void undo_move_piece(int src, int dest, PieceType captured_piece_type);
    void undo_promote(int sq);
    void undo_en_passant(int en_passant_sq);
    void recalculate_all_pieces();
    std::uint64_t get_promotion_row(Color c);
    std::uint64_t get_attackers(int sq, Color attacker_color) const;
    bool is_attacked(int sq, Color attacker_color) const;
    void add_castle_if_legal(Color c, MoveList& list);
    std::uint64_t get_quiets_and_captures(int sq) const;
    std::uint64_t get_legal_quiets_and_captures(int sq);
    std::uint64_t get_legal_castle_moves(Color c);
    std::uint64_t get_legal_en_passant_moves(int sq);
    std::uint64_t get_en_passant_row(Color c);
    void recalculate_pinned(Color c);

    template<Color c>
    void recalculate_pinned();

    std::expected<std::array<std::string_view, 6>, FENError> parse_FEN(const std::string& str);
    
public:
    Board();
    int get_king_pos(Color c) const;
    void populate_legal_moves(int sq, MoveList& list);
    void populate_legal_moves(MoveList& list);
    void make_move(Move move);
    void undo_last_move();
    bool in_check(Color c);
    bool turn_color_in_check();
    void reset();
    BoardState get_board_state();

    std::string layout() const;
    void assert_valid();
    void populate_perft(int depth, PerftResults& stats);
    std::expected<void, FENError> load_FEN(const std::string& str);
    friend struct BoardState;
};


template<Color c>
void Board::recalculate_pinned(){
    using namespace tables::pins;
    constexpr Color enemy_color = other_color(c);
    int king_sq = get_king_pos(c);
    pinned = 0;
    std::uint64_t enemy_queens = bitboards[to_piece(enemy_color, PieceType::QUEEN)];
    std::uint64_t diagonal_attackers = (bitboards[to_piece(enemy_color, PieceType::BISHOP)] | enemy_queens) & diagonal_mask[king_sq];
    std::uint64_t straight_attackers = (bitboards[to_piece(enemy_color, PieceType::ROOK)] | enemy_queens) & straight_mask[king_sq];
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


#endif
