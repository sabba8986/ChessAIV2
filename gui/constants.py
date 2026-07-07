from PySide6.QtGui import QIcon
from board import Piece, Color, PieceType


ICONS = {
    Piece.EMPTY: QIcon(),
    Piece.BLACK_PAWN: QIcon("../assets/icons/pieces/black_pawn.svg"),
    Piece.BLACK_ROOK: QIcon("../assets/icons/pieces/black_rook.svg"),
    Piece.BLACK_KNIGHT: QIcon("../assets/icons/pieces/black_knight.svg"),
    Piece.BLACK_BISHOP: QIcon("../assets/icons/pieces/black_bishop.svg"),
    Piece.BLACK_QUEEN: QIcon("../assets/icons/pieces/black_queen.svg"),
    Piece.BLACK_KING: QIcon("../assets/icons/pieces/black_king.svg"),
    Piece.WHITE_PAWN: QIcon("../assets/icons/pieces/white_pawn.svg"),
    Piece.WHITE_ROOK: QIcon("../assets/icons/pieces/white_rook.svg"),
    Piece.WHITE_KNIGHT: QIcon("../assets/icons/pieces/white_knight.svg"),
    Piece.WHITE_BISHOP: QIcon("../assets/icons/pieces/white_bishop.svg"),
    Piece.WHITE_QUEEN: QIcon("../assets/icons/pieces/white_queen.svg"),
    Piece.WHITE_KING: QIcon("../assets/icons/pieces/white_king.svg")
}

BROWN = "#B88B4A"
WHITE = "#E3C16F"

TILE_STYLE = "background-color:{color}; border: black; border-width: thin"
NON_CAPTURE_STYLE = "background-color:#FFFF00; border: 1px solid black"
CAPTURE_STYLE = "background-color:#FF0000; border: 1px solid black"
CHECK_STYLE = "background-color:#FFA500; border: 1px solid black"



