#include "board_state.hpp"


BoardState::BoardState(Board& board): pieces{board.pieces}, turn{board.turn}, in_check{{board.in_check(WHITE), board.in_check(BLACK)}}{
    for(int sq = 0; sq < 64; sq++){
       moves[sq] = pieces[sq].color == turn ? board.get_legal_moves(sq) : MoveList();
    }
}