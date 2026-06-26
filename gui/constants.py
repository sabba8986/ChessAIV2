from PySide6.QtGui import QIcon
from enum import IntEnum


ICONS = {
    "_": QIcon(),
    "p": QIcon("../assets/icons/pieces/black_pawn.svg"),
    "r": QIcon("../assets/icons/pieces/black_rook.svg"),
    "n": QIcon("../assets/icons/pieces/black_knight.svg"),
    "b": QIcon("../assets/icons/pieces/black_bishop.svg"),
    "q": QIcon("../assets/icons/pieces/black_queen.svg"),
    "k": QIcon("../assets/icons/pieces/black_king.svg"),
    "P": QIcon("../assets/icons/pieces/white_pawn.svg"),
    "R": QIcon("../assets/icons/pieces/white_rook.svg"),
    "N": QIcon("../assets/icons/pieces/white_knight.svg"),
    "B": QIcon("../assets/icons/pieces/white_bishop.svg"),
    "Q": QIcon("../assets/icons/pieces/white_queen.svg"),
    "K": QIcon("../assets/icons/pieces/white_king.svg")
}

BROWN = "#B88B4A"
WHITE = "#E3C16F"

TILE_STYLE = "background-color:{color}; border: black; border-width: thin"
NON_CAPTURE_STYLE = "background-color:#FFFF00; border: 1px solid black"
CAPTURE_STYLE = "background-color:#FF0000; border: 1px solid black"
CHECK_STYLE = "background-color:#FFA500; border: 1px solid black"

class Color(IntEnum):
    WHITE = 0
    BLACK = 1

class PieceType(IntEnum):
    PAWN = 0
    ROOK = 1
    KNIGHT = 2
    BISHOP = 3
    QUEEN = 4
    KING = 5
    EMPTY = 6


