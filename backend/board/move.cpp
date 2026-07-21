#include "move.hpp"
#include <cassert>


Move::Move(int src, int dest, std::uint16_t flags, PieceType promoted_type):
    move(src | (dest << 6) | flags | (flags & promotion_flag ? (static_cast<std::uint16_t>(promoted_type) - 2) << 14 : 0)){}


int Move::src(){
    return move & src_mask;
}


int Move::dest(){
    return (move & dest_mask) >> 6;
}


bool Move::is_en_passant(){
    return (move & en_passant_flag) && !is_promotion();
}


bool Move::is_promotion(){
    return move & promotion_flag;
}


PieceType Move::promoted_type(){
    assert(is_promotion() && "Move must be a promotion");
    return is_promotion() ? static_cast<PieceType>(((move & promotion_mask) >> 14) + 2) : PieceType::EMPTY;
}


void Move::set_promoted_type(PieceType p){
    assert(((p == PieceType::QUEEN) | (p == PieceType::KNIGHT) | (p == PieceType::BISHOP) | (p == PieceType::ROOK)) && "Must be a valid piecetype");
    move &= ~promotion_mask;
    move |= (static_cast<std::uint16_t>(p) - 2) << 14;
}


bool Move::is_castle(){
    return (move & castle_flag) && !is_promotion();
}

bool Move::is_capture(){
    return move & capture_flag;
}

bool Move::is_quiet(){
    return !is_capture();
}