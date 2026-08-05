#include "board_state.hpp"
#include "board.hpp"

BoardState::BoardState(Board& board): m_pieces{board.pieces.to_array()}, m_in_check{{board.in_check(Color::WHITE), board.in_check(Color::BLACK)}}, m_move_lists{}{
    bool no_legal_moves = true;
    for(int sq = 0; sq < 64; sq++){
        board.populate_legal_moves(sq, m_move_lists[sq]);
        if((m_pieces[sq] != Piece::EMPTY) && (get_color(m_pieces[sq]) == board.turn) && (m_move_lists[sq].size() != 0)){
            no_legal_moves = false;
        }
    }
    if(no_legal_moves){
        m_game_state = board.turn_color_in_check() ? GameState::CHECKMATE : GameState::STALEMATE;
    }
    else if(board.clock == 100){
        m_game_state = GameState::DRAW_HUNDRED_MOVE_CLOCK;
    }
    else{
        m_game_state = GameState::ONGOING;
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

GameState BoardState::game_state(){
    return m_game_state;
}