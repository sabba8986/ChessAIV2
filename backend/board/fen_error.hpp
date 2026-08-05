#ifndef FEN_ERROR
#define FEN_ERROR
#include <string>

enum class FENError{
    MISSING_SECTION,
    EXTRA_SECTION,
    ZERO_IN_BOARD,
    NOT_ENOUGH_COLUMNS,
    NOT_ENOUGH_ROWS,
    INVALID_CHARACTER_IN_ROW,
    DUPLICATE_KING,
    MISSING_KING,
    INVALID_ACTIVE_COLOR, 
    INVALID_CASTLE_RIGHTS,
    DUPLICATE_CASTLE_RIGHTS,
    INVALID_EN_PASSANT_SQ,
    EN_PASSANT_SQ_OUT_OF_BOUNDS,
    INVALID_HALF_MOVE_CLOCK,
    INVALID_NUM_MOVES
};


inline std::string string_rep(FENError err){
    switch(err){
        case FENError::MISSING_SECTION: return "Missing one or more sections";
        case FENError::EXTRA_SECTION: return "Added one or more sections than necessary";
        case FENError::ZERO_IN_BOARD: return "Piece placement contains 0";
        case FENError::NOT_ENOUGH_COLUMNS: return "Piece placement is missing one or more columns";
        case FENError::NOT_ENOUGH_ROWS: return "Piece placement is missing one or more rows";
        case FENError::INVALID_CHARACTER_IN_ROW: return "Piece placement contains invalid character";
        case FENError::DUPLICATE_KING: return "Piece placement contains duplicate king of same color";
        case FENError::MISSING_KING: return "Piece placement is missing king of either color";
        case FENError::INVALID_ACTIVE_COLOR: return "Active color contains invalid character";
        case FENError::INVALID_CASTLE_RIGHTS: return "Castle rights contains invalid character";
        case FENError::DUPLICATE_CASTLE_RIGHTS: return "Castle rights contains duplicate character";
        case FENError::INVALID_EN_PASSANT_SQ: return "En passant square contains invalid (not numeric) character";
        case FENError::EN_PASSANT_SQ_OUT_OF_BOUNDS: return "En passant square is out of bounds of the board";
        case FENError::INVALID_HALF_MOVE_CLOCK: return "Half move clock contains invalid (not numeric) character";
        case FENError::INVALID_NUM_MOVES: return "Number of full moves contains invalid (not numeric) character";
        default: return "";
    }
}


#endif