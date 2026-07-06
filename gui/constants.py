from PySide6.QtGui import QIcon
from board import Piece, Color, PieceType


ICONS = {
    Piece(Color.BLACK, PieceType.EMPTY): QIcon(),
    Piece(Color.BLACK, PieceType.PAWN): QIcon("../assets/icons/pieces/black_pawn.svg"),
    Piece(Color.BLACK, PieceType.ROOK): QIcon("../assets/icons/pieces/black_rook.svg"),
    Piece(Color.BLACK, PieceType.KNIGHT): QIcon("../assets/icons/pieces/black_knight.svg"),
    Piece(Color.BLACK, PieceType.BISHOP): QIcon("../assets/icons/pieces/black_bishop.svg"),
    Piece(Color.BLACK, PieceType.QUEEN): QIcon("../assets/icons/pieces/black_queen.svg"),
    Piece(Color.BLACK, PieceType.KING): QIcon("../assets/icons/pieces/black_king.svg"),
    Piece(Color.WHITE, PieceType.PAWN): QIcon("../assets/icons/pieces/white_pawn.svg"),
    Piece(Color.WHITE, PieceType.ROOK): QIcon("../assets/icons/pieces/white_rook.svg"),
    Piece(Color.WHITE, PieceType.KNIGHT): QIcon("../assets/icons/pieces/white_knight.svg"),
    Piece(Color.WHITE, PieceType.BISHOP): QIcon("../assets/icons/pieces/white_bishop.svg"),
    Piece(Color.WHITE, PieceType.QUEEN): QIcon("../assets/icons/pieces/white_queen.svg"),
    Piece(Color.WHITE, PieceType.KING): QIcon("../assets/icons/pieces/white_king.svg")
}

BROWN = "#B88B4A"
WHITE = "#E3C16F"

TILE_STYLE = "background-color:{color}; border: black; border-width: thin"
NON_CAPTURE_STYLE = "background-color:#FFFF00; border: 1px solid black"
CAPTURE_STYLE = "background-color:#FF0000; border: 1px solid black"
CHECK_STYLE = "background-color:#FFA500; border: 1px solid black"



