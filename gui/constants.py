from PySide6.QtGui import QColor, QPen, QBrush, QIcon
from PySide6.QtSvg import QSvgRenderer
from PySide6.QtCore import Qt
from pathlib import Path
from board import Piece, PieceType


iconsDir = (Path(__file__).resolve().parent.parent) / "assets" / "icons" / "pieces"


PIECE_RENDERERS: list[QSvgRenderer] = [QSvgRenderer()] * 14


def initializePieceRenderers():
    PIECE_RENDERERS[Piece.EMPTY] = QSvgRenderer()
    PIECE_RENDERERS[Piece.WHITE_PAWN] = QSvgRenderer(str(iconsDir / "white_pawn.svg"))
    PIECE_RENDERERS[Piece.WHITE_ROOK] = QSvgRenderer(str(iconsDir / "white_rook.svg"))
    PIECE_RENDERERS[Piece.WHITE_KNIGHT] = QSvgRenderer(str(iconsDir / "white_knight.svg"))
    PIECE_RENDERERS[Piece.WHITE_BISHOP] = QSvgRenderer(str(iconsDir / "white_bishop.svg"))
    PIECE_RENDERERS[Piece.WHITE_QUEEN] = QSvgRenderer(str(iconsDir / "white_queen.svg"))
    PIECE_RENDERERS[Piece.WHITE_KING] = QSvgRenderer(str(iconsDir / "white_king.svg"))
    PIECE_RENDERERS[Piece.BLACK_PAWN] = QSvgRenderer(str(iconsDir / "black_pawn.svg"))
    PIECE_RENDERERS[Piece.BLACK_ROOK] = QSvgRenderer(str(iconsDir / "black_rook.svg"))
    PIECE_RENDERERS[Piece.BLACK_KNIGHT] = QSvgRenderer(str(iconsDir / "black_knight.svg"))
    PIECE_RENDERERS[Piece.BLACK_BISHOP] = QSvgRenderer(str(iconsDir / "black_bishop.svg"))
    PIECE_RENDERERS[Piece.BLACK_QUEEN] = QSvgRenderer(str(iconsDir / "black_queen.svg"))
    PIECE_RENDERERS[Piece.BLACK_KING] = QSvgRenderer(str(iconsDir / "black_king.svg"))


WHITE_PROMOTION_ICONS: list[QIcon] = [ \
    QIcon(), \
    QIcon(), \
    QIcon(str(iconsDir / "white_rook.svg")), \
    QIcon(str(iconsDir / "white_knight.svg")), \
    QIcon(str(iconsDir / "white_bishop.svg")), \
    QIcon(str(iconsDir / "white_queen.svg")) \
]


BLACK_PROMOTION_ICONS: list[QIcon] = [ \
    QIcon(), \
    QIcon(), \
    QIcon(str(iconsDir / "black_rook.svg")), \
    QIcon(str(iconsDir / "black_knight.svg")), \
    QIcon(str(iconsDir / "black_bishop.svg")), \
    QIcon(str(iconsDir / "black_queen.svg")) \
]


TILE_PEN: QPen = QPen(QColor(0, 0, 0, 10))
SELECTED_PEN: QPen = QPen(QBrush(Qt.BrushStyle.SolidPattern), 2)
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



