#ifndef BOARD
#define BOARD

#include "piece.hpp"
#include "move.hpp"
#include "move_list.hpp"
#include "enum_arr.hpp"
#include "history.hpp"
#include <cstdint>
#include <array>
#include <bit>
#include "board_state.hpp"
#include "perft_results.hpp"
#include <string>
#include "fen_error.hpp"
#include <expected>

class Board{
    static constexpr std::size_t MAX_PLYS = 128;
    static constexpr std::uint8_t white_left_castle_allowed_flag = 1;
    static constexpr std::uint8_t white_right_castle_allowed_flag = 2;
    static constexpr std::uint8_t black_left_castle_allowed_flag = 4;
    static constexpr std::uint8_t black_right_castle_allowed_flag = 8;
    
    EnumArr<std::uint64_t, 15> bitboards;
    EnumArr<std::uint64_t, 2> all_pieces;
    EnumArr<Piece, 64> pieces;
    std::uint64_t pinned;
    std::uint64_t checkers;
    std::array<History, MAX_PLYS> prev_moves;
    int en_passant_sq;
    std::uint8_t castle_rights;
    int clock;
    Color turn;
    std::size_t num_moves;
    std::size_t cur_ply;

    template<Color c>
    void do_castle(int src, int dest);

    template<Color c>
    void move_piece(int src, int dest);

    template<Color c>
    void promote(int sq, PieceType p);

    template<Color c>
    void do_en_passant(int en_passant_sq);

    template<Color c>
    void undo_castle(int src, int dest);

    template<Color c>
    void undo_move_piece(int src, int dest, Piece captured_piece);

    template<Color c>
    void undo_promote(int sq);

    template<Color c>
    void undo_en_passant(int en_passant_sq);


    void recalculate_all_pieces();

    template<Color attacker_color>
    std::uint64_t get_attackers(int sq);

    template<Color attacker_color>
    bool is_attacked(int sq);

    template<Color c>
    std::uint64_t get_checkers();

    template<Color c>
    std::uint64_t get_quiets_and_captures(int sq);

    template<Color c>
    std::uint64_t get_legal_quiets_and_captures(int sq);

    template<Color c>
    std::uint64_t get_legal_castle_moves();

    template<Color c>
    std::uint64_t get_legal_en_passant_moves(int sq);

    template<Color c>
    void populate_legal_moves(int sq, MoveList& list);

    void recalculate_pinned(Color c);
    void perft_helper(int depth, PerftResults& stats);
    std::uint64_t fast_perft_helper(int depth);
    
    template<Color c>
    void recalculate_pinned();

    std::expected<std::array<std::string_view, 6>, FENError> parse_FEN(const std::string& str);

    template<Color c>
    void make_move(Move move);

    template<Color c>
    void undo_last_move();

    template<Color c>
    static std::uint64_t get_pawn_starting_row();

    template<Color c>
    static std::uint64_t get_pawn_double_push_row();

    template<Color c>
    static std::uint64_t get_en_passant_row();

    template<Color c>
    static std::uint64_t get_promotion_row();

    template<Color c>
    int get_king_pos();

    
public:
    Board();
    constexpr static std::size_t get_max_plys(){ return MAX_PLYS; }

    void populate_legal_moves(int sq, MoveList& list);
    void populate_legal_moves(MoveList& list);
    void make_move(Move move);
    void undo_last_move();

    int get_king_pos(Color c);

    template<Color c>
    bool in_check();

    bool in_check(Color c);


    bool turn_color_in_check();
    void reset();
    BoardState get_board_state();
    std::size_t get_cur_ply();


    std::string layout();
    void assert_valid();
    PerftResults perft(int depth);
    std::uint64_t fast_perft(int depth);
    std::expected<void, FENError> load_FEN(const std::string& str);
    friend struct BoardState;
};


#include "board.inl"

#endif
