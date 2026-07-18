#ifndef BOARD
#define BOARD

#include "piece.hpp"
#include "move.hpp"
#include "move_list.hpp"
#include "history.hpp"
#include <cstdint>
#include <array>
#include <stack>
#include "board_state.hpp"


class Board{
    static constexpr std::uint8_t white_left_castle_allowed_flag = 1;
    static constexpr std::uint8_t white_right_castle_allowed_flag = 2;
    static constexpr std::uint8_t black_left_castle_allowed_flag = 4;
    static constexpr std::uint8_t black_right_castle_allowed_flag = 8;
    
    //bitboards for each color ({WHITE, BLACK}). Pieces are in the order of: 
    //{Pawn, Rook, Knight, Bishop, Queen, King, Empty}
    std::array<std::uint64_t, 14> bitboards;
    std::array<std::uint64_t, 2> all_pieces;
    std::array<Piece, 64> pieces;
    int en_passant_sq;
    std::uint8_t castle_rights;
    int clock;
    std::stack<History> prev_moves;
    Color turn;


    int get_king_pos(Color c) const;
    std::uint64_t get_checkers(Color c) const;
    std::uint64_t pinner(int sq);

    void do_castle(int src, int dest);
    void move_piece(int src, int dest);
    void promote(int sq, PieceType p);
    void do_en_passant(int en_passant_sq);
    void undo_castle(int src, int dest);
    void undo_move_piece(int src, int dest, PieceType captured_piece_type);
    void undo_promote(int sq, PieceType p);
    void undo_en_passant(int en_passant_sq);
    void recalculate_all_pieces();
    std::uint64_t get_promotion_row(Color c);
    std::uint64_t get_attackers(int sq, Color attacker_color) const;
    void add_castle_if_legal(Color c, MoveList& list);
    std::uint64_t get_quiets_and_captures(int sq) const;
    std::uint64_t get_legal_quiets_and_captures(int sq);
    std::uint64_t get_castle_moves(Color c);
    std::uint64_t get_en_passant_row(Color c);
public:
    Board();
    MoveList get_legal_moves(int sq);
    void make_move(Move move);
    void undo_last_move();
    bool in_check(Color c);
    void reset();
    BoardState get_board_state();

    std::string layout() const;
    void assert_valid() const;
    friend struct BoardState;
};

#endif
