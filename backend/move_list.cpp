#include "move_list.hpp"


MoveList::MoveList(): num_moves{0}{}

void MoveList::add_move(Move move){
    moves[num_moves++] = move;
}