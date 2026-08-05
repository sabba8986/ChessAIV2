#include "move_list.hpp"
#include <cassert>

MoveList::MoveList(): m_num_moves{0}{}


void MoveList::add_move(Move move){
    m_moves[m_num_moves++] = move;
}


int MoveList::size(){
    return m_num_moves;
}


Move MoveList::operator[](int move_num){
    assert(move_num >= 0 && move_num < size() && "Indexing out of bounds");
    return m_moves[move_num];
}
