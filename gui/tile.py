from board import Move
from constants import BROWN_BRUSH, WHITE_BRUSH, isWhite, topLeft
from PySide6.QtCore import QPoint, QRect, QSize
from PySide6.QtGui import QBrush


class Tile:
    def __init__(self, sq: int):
        self.sq = sq
        self.brush: QBrush = WHITE_BRUSH if isWhite(sq) else BROWN_BRUSH
        self.topLeft: QPoint = QPoint(topLeft(sq, 0))
        self.rect: QRect = QRect(self.topLeft, QSize(0, 0))
        self.moves: list[Move] = []

    def resize(self, tileLength: int):
        self.topLeft = QPoint(topLeft(self.sq, tileLength))
        self.rect = QRect(self.topLeft, QSize(tileLength, tileLength))