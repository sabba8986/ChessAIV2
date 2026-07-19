from PySide6.QtGui import QColor, QPen, QBrush
from PySide6.QtSvg import QSvgRenderer
from pathlib import Path
from board import Piece


iconsDir = (Path(__file__).resolve().parent.parent) / "assets" / "icons" / "pieces"


ICONS: list[QSvgRenderer | None] = [None] * 14


def initializeIcons():
    ICONS[Piece.EMPTY] = QSvgRenderer()
    ICONS[Piece.WHITE_PAWN] = QSvgRenderer(str(iconsDir / "white_pawn.svg"))
    ICONS[Piece.WHITE_ROOK] = QSvgRenderer(str(iconsDir / "white_rook.svg"))
    ICONS[Piece.WHITE_KNIGHT] = QSvgRenderer(str(iconsDir / "white_knight.svg"))
    ICONS[Piece.WHITE_BISHOP] = QSvgRenderer(str(iconsDir / "white_bishop.svg"))
    ICONS[Piece.WHITE_QUEEN] = QSvgRenderer(str(iconsDir / "white_queen.svg"))
    ICONS[Piece.WHITE_KING] = QSvgRenderer(str(iconsDir / "white_king.svg"))
    ICONS[Piece.BLACK_PAWN] = QSvgRenderer(str(iconsDir / "black_pawn.svg"))
    ICONS[Piece.BLACK_ROOK] = QSvgRenderer(str(iconsDir / "black_rook.svg"))
    ICONS[Piece.BLACK_KNIGHT] = QSvgRenderer(str(iconsDir / "black_knight.svg"))
    ICONS[Piece.BLACK_BISHOP] = QSvgRenderer(str(iconsDir / "black_bishop.svg"))
    ICONS[Piece.BLACK_QUEEN] = QSvgRenderer(str(iconsDir / "black_queen.svg"))
    ICONS[Piece.BLACK_KING] = QSvgRenderer(str(iconsDir / "black_king.svg"))


BLACK: QColor = QColor(0, 0, 0, 0.30)
BROWN: QColor = QColor(0xB88B4A)
WHITE: QColor = QColor(0xE3C16F)


BLACK_PEN: QPen = QPen(BLACK)


BROWN_BRUSH: QBrush = QBrush(BROWN)
WHITE_BRUSH: QBrush = QBrush(WHITE)


TILE_STYLE = "background-color:{color}; border: black; border-width: thin"
NON_CAPTURE_STYLE = "background-color:#FFFF00; border: 1px solid black"
CAPTURE_STYLE = "background-color:#FF0000; border: 1px solid black"
CHECK_STYLE = "background-color:#FFA500; border: 1px solid black"



