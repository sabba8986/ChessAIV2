#include "move.hpp"


Move::Move(int from, int to, PieceType promoted_type): 
    move(from | 
        (to << 6) | 
        promotion_flag | 
        promoted_type << 13){}


Move::Move(int from, int to, std::uint16_t flags): 
    move(from | 
        (to << 6) | 
        flags){}


int Move::from(){
    return move & from_mask;
}


int Move::to(){
    return (move & to_mask) >> 6;
}


bool Move::is_en_passant(){
    return move & en_passant_flag;
}


bool Move::is_promotion(){
    return move & promotion_flag;
}


PieceType Move::promoted_type(){
    return static_cast<PieceType>((move & promotion_mask) >> 13);
}


bool Move::is_castle(){
    return move & castle_flag;
}