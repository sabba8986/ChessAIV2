#include "move_list.hpp"


MoveList::MoveList(): m_num_moves{0}, m_cur_move_idx{0}, m_moves{} {}


void MoveList::add_move(Move move){
    m_moves[m_num_moves++] = move;
}


int MoveList::size(){
    return m_num_moves;
}


Move MoveList::operator[](int move_num){
    return m_moves[move_num];
}