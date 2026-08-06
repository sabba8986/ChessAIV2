#pragma once
#include "piece.hpp"
#include "move_list.hpp"
#include "enum_arr.hpp"
#include "game_state.hpp"
#include <array>

class Board;


struct BoardState{
private:
    EnumArr<bool, 2> m_in_check;
    std::array<MoveList, 64> m_move_lists;
    std::array<Piece, 64> m_pieces;
    GameState m_game_state;


public: 
    BoardState(Board& board);
    bool in_check(Color c);
    MoveList move_list(int sq);
    Piece piece(int sq);
    GameState game_state();
};
