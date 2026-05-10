#include "constants.hpp"
#include "Board.hpp"

#include <string>
#include <iostream>

Board::Board(){
    white[PAWN_INDEX] = INIT_WHITE_PAWNS;
    white[ROOK_INDEX] = INIT_WHITE_ROOKS;
    white[KNIGHT_INDEX] = INIT_WHITE_KNIGHTS;
    white[BISHOP_INDEX] = INIT_WHITE_BISHOPS;
    white[QUEEN_INDEX] = INIT_WHITE_QUEEN;
    white[KING_INDEX] = INIT_WHITE_KING;

    black[PAWN_INDEX] = INIT_BLACK_PAWNS;
    black[ROOK_INDEX] = INIT_BLACK_ROOKS;
    black[KNIGHT_INDEX] = INIT_BLACK_KNIGHTS;
    black[BISHOP_INDEX] = INIT_BLACK_BISHOPS;
    black[QUEEN_INDEX] = INIT_BLACK_QUEEN;
    black[KING_INDEX] = INIT_BLACK_KING;
}

const char *Board::board_as_string(){
    char *board_as_string = new char[64]; 

    std::uint64_t cur_piece = BITBOARD_TOP_LEFT;
    for(int i = 0; i < 64; i++, cur_piece >>= 1){
        if(cur_piece & white[KING_INDEX]){
            board_as_string[i] = 'K';
        }
        else if(cur_piece & white[QUEEN_INDEX]){
            board_as_string[i] = 'Q';
        }
        else if(cur_piece & white[KNIGHT_INDEX]){
            board_as_string[i] = 'N';
        }
        else if(cur_piece & white[BISHOP_INDEX]){
            board_as_string[i] = 'B';
        }
        else if(cur_piece & white[ROOK_INDEX]){
            board_as_string[i] = 'R';
        }
        else if(cur_piece & white[PAWN_INDEX]){
            board_as_string[i] = 'P';
        }
        else if(cur_piece & black[KING_INDEX]){
            board_as_string[i] = 'k';
        }
        else if(cur_piece & black[QUEEN_INDEX]){
            board_as_string[i] = 'q';
        }
        else if(cur_piece & black[KNIGHT_INDEX]){
            board_as_string[i] = 'n';
        }
        else if(cur_piece & black[ROOK_INDEX]){
            board_as_string[i] = 'r';
        }
        else if(cur_piece & black[BISHOP_INDEX]){
            board_as_string[i] = 'b';
        }
        else if(cur_piece & black[PAWN_INDEX]){
            board_as_string[i] = 'p';
        } 
        else{
            board_as_string[i] = '_';
        }
    } 
    return board_as_string;
}


std::uint64_t Board::all_attacks(int sq){
    if(){
    }
}
