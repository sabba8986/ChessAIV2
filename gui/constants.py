from PySide6.QtGui import QIcon
from board import Piece
from pathlib import Path

iconsDir = (Path(__file__).resolve().parent.parent) / "assets" / "icons" / "pieces"

ICONS = {
    Piece.EMPTY: QIcon(),
    Piece.BLACK_PAWN: QIcon(str(iconsDir / "black_pawn.svg")),
    Piece.BLACK_ROOK: QIcon(str(iconsDir / "black_rook.svg")),
    Piece.BLACK_KNIGHT: QIcon(str(iconsDir / "black_knight.svg")),
    Piece.BLACK_BISHOP: QIcon(str(iconsDir / "black_bishop.svg")),
    Piece.BLACK_QUEEN: QIcon(str(iconsDir / "black_queen.svg")),
    Piece.BLACK_KING: QIcon(str(iconsDir / "black_king.svg")),
    Piece.WHITE_PAWN: QIcon(str(iconsDir / "white_pawn.svg")),
    Piece.WHITE_ROOK: QIcon(str(iconsDir / "white_rook.svg")),
    Piece.WHITE_KNIGHT: QIcon(str(iconsDir / "white_knight.svg")),
    Piece.WHITE_BISHOP: QIcon(str(iconsDir / "white_bishop.svg")),
    Piece.WHITE_QUEEN: QIcon(str(iconsDir / "white_queen.svg")),
    Piece.WHITE_KING: QIcon(str(iconsDir / "white_king.svg"))
}

BROWN = "#B88B4A"
WHITE = "#E3C16F"

TILE_STYLE = "background-color:{color}; border: black; border-width: thin"
NON_CAPTURE_STYLE = "background-color:#FFFF00; border: 1px solid black"
CAPTURE_STYLE = "background-color:#FF0000; border: 1px solid black"
CHECK_STYLE = "background-color:#FFA500; border: 1px solid black"



