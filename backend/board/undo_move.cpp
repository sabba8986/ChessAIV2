#include "undo_move.hpp"

UndoMove::UndoMove(int en_passant_sq, PieceType captured_piece_type, std::uint8_t castle_rights, int clock): 
    m_en_passant_sq{en_passant_sq}, m_captured_piece_type{captured_piece_type}, m_castle_rights{castle_rights}, m_clock{clock}{}


UndoMove::UndoMove(UndoMove&& other): 
    m_en_passant_sq{other.m_en_passant_sq}, m_captured_piece_type{other.m_captured_piece_type}, m_castle_rights{other.m_castle_rights}, 
    m_clock{other.m_clock}{}


UndoMove& UndoMove::operator=(UndoMove&& other){
    m_en_passant_sq = other.m_en_passant_sq;
    m_captured_piece_type = other.m_captured_piece_type;
    m_castle_rights = other.m_castle_rights;
    m_clock = other.m_clock;
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


