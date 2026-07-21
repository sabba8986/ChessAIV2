from PySide6.QtGui import QBrush
from PySide6.QtCore import QRect
from constants import WHITE_BRUSH, BROWN_BRUSH
from board import Move


def isWhiteTile(n):
    row: int = n // 8
    return (n % 2 == 0) if (row % 2 == 0) else (n % 2 != 0)


class Tile:
    def __init__(self, tileNum: int, rect: QRect | None = None):
        self.brush: QBrush = WHITE_BRUSH if isWhiteTile(tileNum) else BROWN_BRUSH
        self.rect: QRect = QRect()
        self.move: Move | None = None