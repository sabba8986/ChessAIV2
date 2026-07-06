#include "move_list.hpp"


MoveList::MoveList(): num_moves{0} {}


void MoveList::add_move(Move move){
    moves[num_moves++] = move;
}


int MoveList::size(){
    return num_moves;
}


Move MoveList::operator[](int move_num){
    return moves[move_num];
}