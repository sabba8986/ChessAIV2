#include "move.hpp"

Move::Move(int src, int dest, std::uint16_t flags, PieceType promoted_type):
    move(src | (dest << 6) | flags | (flags & promotion_flag ? (static_cast<std::uint16_t>(promoted_type) - 1) << 14 : 0)){}


int Move::src(){
    return move & src_mask;
}


int Move::dest(){
    return (move & dest_mask) >> 6;
}


bool Move::is_en_passant(){
    return (move & en_passant_flag) & !(move & promotion_flag);
}


bool Move::is_promotion(){
    return move & promotion_flag;
}


PieceType Move::promoted_type(){
    return is_promotion() ? static_cast<PieceType>(((move & promotion_mask) >> 14) + 1) : EMPTY;
}


bool Move::is_castle(){
    return (move & castle_flag) & !(move & promotion_flag);
}

bool Move::is_capture(){
    return move & capture_flag;
}

bool Move::is_quiet(){
    return !is_capture();
}