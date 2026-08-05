#include "undo_move.hpp"

UndoMove::UndoMove(int en_passant_sq, PieceType captured_piece_type, std::uint8_t castle_rights, int clock, std::uint64_t prev_enemy_pinned, std::uint64_t prev_checkers): 
    m_en_passant_sq{en_passant_sq}, m_captured_piece_type{captured_piece_type}, m_castle_rights{castle_rights}, m_clock{clock}, m_prev_enemy_pinned{prev_enemy_pinned}, m_prev_checkers{prev_checkers}{}


UndoMove::UndoMove(UndoMove&& other): 
    m_en_passant_sq{other.m_en_passant_sq}, m_captured_piece_type{other.m_captured_piece_type}, m_castle_rights{other.m_castle_rights}, m_prev_enemy_pinned{other.m_prev_enemy_pinned},
    m_clock{other.m_clock}, m_prev_checkers{other.m_prev_checkers}{}


UndoMove& UndoMove::operator=(UndoMove&& other){
    m_en_passant_sq = other.m_en_passant_sq;
    m_captured_piece_type = other.m_captured_piece_type;
    m_castle_rights = other.m_castle_rights;
    m_clock = other.m_clock;
    m_prev_enemy_pinned = other.m_prev_enemy_pinned;
    m_prev_checkers = other.m_prev_checkers;
    return *this;
}


int UndoMove::en_passant_sq(){
    return m_en_passant_sq;
}


PieceType UndoMove::captured_piece_type(){
    return m_captured_piece_type;
}


std::uint8_t UndoMove::castle_rights(){
    return m_castle_rights;
}


int UndoMove::clock(){
    return m_clock;
}


std::uint64_t UndoMove::prev_enemy_pinned(){
    return m_prev_enemy_pinned;
}


std::uint64_t UndoMove::prev_checkers(){
    return m_prev_checkers;
}