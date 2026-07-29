#include "board_state.hpp"
#include "board.hpp"

BoardState::BoardState(Board& board): m_pieces{board.pieces.to_array()}, m_in_check{{board.in_check(Color::WHITE), board.in_check(Color::BLACK)}}{
    for(int sq = 0; sq < 64; sq++){
       m_move_lists[sq] = board.get_legal_moves(sq);
    }
}


bool BoardState::in_check(Color c){
    return m_in_check[c];
}


MoveList BoardState::move_list(int sq){
    return m_move_lists[sq];
}


Piece BoardState::piece(int sq){
    return m_pieces[sq];
}