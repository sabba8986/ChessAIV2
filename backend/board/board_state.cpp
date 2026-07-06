#include "board_state.hpp"
#include "board.hpp"

BoardState::BoardState(Board& board): m_pieces{board.pieces}, m_in_check{{board.in_check(WHITE), board.in_check(BLACK)}}{
    for(int sq = 0; sq < 64; sq++){
       m_move_lists[sq] = m_pieces[sq].color == board.turn ? board.get_legal_moves(sq) : MoveList();
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