from pathlib import Path

from board import Piece
from PySide6.QtCore import QPoint, Qt
from PySide6.QtGui import QBrush, QColor, QIcon, QPen
from PySide6.QtSvg import QSvgRenderer

iconsDir = (Path(__file__).resolve().parent.parent) / "assets" / "icons" / "pieces"


def isWhite(sq: int):
    row: int = sq // 8
    return (sq % 2 == 0) if (row % 2 == 0) else (sq % 2 != 0)


def toPos(sq: int) -> tuple[int, int]:
    return ((63 - sq) % 8, (63 - sq) // 8)


def topLeft(sq: int, tileLength: int) -> QPoint:
    tup: tuple[int, int] = toPos(sq)
    return QPoint(tileLength * tup[0], tileLength * tup[1])


def cursorTileTopLeft(point: QPoint, tileLength: int) -> QPoint:
    return point - QPoint(tileLength // 2, tileLength // 2)


PIECE_PATHS: list[str] = [
    "", 
    str(iconsDir / "white_pawn.svg"),
    str(iconsDir / "white_rook.svg"),
    str(iconsDir / "white_knight.svg"), 
    str(iconsDir / "white_bishop.svg"),
    str(iconsDir / "white_queen.svg"),
    str(iconsDir / "white_king.svg"), 
    "", 
    str(iconsDir / "black_pawn.svg"),
    str(iconsDir / "black_rook.svg"),
    str(iconsDir / "black_knight.svg"),
    str(iconsDir / "black_bishop.svg"),
    str(iconsDir / "black_queen.svg"),
    str(iconsDir / "black_king.svg")
]


PIECE_RENDERERS: list[QSvgRenderer] = [QSvgRenderer()] * 14
def initializePieceRenderers():
    for piece in Piece: 
        if piece != Piece.EMPTY:
            PIECE_RENDERERS[piece] = QSvgRenderer(PIECE_PATHS[piece])


WHITE_PROMOTION_ICONS: list[QIcon] = [ \
    QIcon(), \
    QIcon(), \
    QIcon(PIECE_PATHS[Piece.WHITE_ROOK]), \
    QIcon(PIECE_PATHS[Piece.WHITE_KNIGHT]), \
    QIcon(PIECE_PATHS[Piece.WHITE_BISHOP]), \
    QIcon(PIECE_PATHS[Piece.WHITE_QUEEN]) \
]


BLACK_PROMOTION_ICONS: list[QIcon] = [ \
    QIcon(), \
    QIcon(), \
    QIcon(PIECE_PATHS[Piece.BLACK_ROOK]), \
    QIcon(PIECE_PATHS[Piece.BLACK_KNIGHT]), \
    QIcon(PIECE_PATHS[Piece.BLACK_BISHOP]), \
    QIcon(PIECE_PATHS[Piece.BLACK_QUEEN]) \
]


TILE_PEN: QPen = QPen(QColor(0, 0, 0, 10))
HIGHLIGHTED_PEN = QPen(QColor(0))


BROWN_BRUSH: QBrush = QBrush(QColor(0xB88B4A))
WHITE_BRUSH: QBrush = QBrush(QColor(0xE3C16F))


CAPTURE_BRUSH: QBrush = QBrush(QColor(0xFF0000))
NON_CAPTURE_BRUSH: QBrush = QBrush(QColor(0xFFFF00))
CHECK_BRUSH: QBrush = QBrush(QColor(0xFFA500))

TILE_STYLE = "background-color:{color}; border: black; border-width: thin"
NON_CAPTURE_STYLE = "background-color:#FFFF00; border: 1px solid black"
CAPTURE_STYLE = "background-color:#FF0000; border: 1px solid black"
CHECK_STYLE = "background-color:#FFA500; border: 1px solid black"



