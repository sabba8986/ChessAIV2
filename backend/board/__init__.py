import os
import sys

libpath = os.environ.get("CHESS_CLIB")
if(libpath):
    if(os.path.isdir(libpath)):
        os.add_dll_directory(libpath)
    else:
        raise ImportError("Invalid directory provided in CHESS_CLIB, aborting import")

from .board_interface import (
    BoardState,
    Color,
    GameState,
    Move,
    MoveList,
    Piece,
    PieceType,
    get_board_state,
    in_check,
    layout,
    make_move,
    reset,
    undo_last_move,
    loadFEN
)


__all__ = ["BoardState", "Color", "GameState", "Move", "MoveList", "Piece", "PieceType", "get_board_state", "in_check", "layout", "loadFEN", "make_move", "reset", "undo_last_move"]