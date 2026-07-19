from PySide6 import QtCore, QtWidgets, QtGui
import board
from board import Piece, Color, Move
from constants import ICONS, TILE_STYLE, CAPTURE_STYLE, NON_CAPTURE_STYLE, CHECK_STYLE, BLACK_PEN, BROWN_BRUSH, WHITE_BRUSH


def numToPos(sq):
    return ((63 - sq) // 8, (63 - sq) % 8)


class Tile(QtWidgets.QPushButton): 
    def __init__(self, window: QtWidgets.QWidget, pos: tuple[int, int]):
        super().__init__(window)
        self.move = None
        self.tilePos = pos
        self.tileNum = (8 * pos[0]) + pos[1]
        # self.tileColor = WHITE if ((pos[0] + pos[1]) % 2 == 0) else BROWN
        self.setFixedSize(QtCore.QSize(100, 100))
        self.setIconSize(QtCore.QSize(100, 100))
        self.setStyleSheet(TILE_STYLE.format(color = self.tileColor)) 
        self.setFlat(True)
        self.clicked.connect(self.onSelectTile)

    def onSelectTile(self):
        if self.move is None:
            self.window().selectedSquare = self.tileNum
            self.window().renderAttacks(self.tileNum)
        else:
            board.make_move(self.move)
            self.window().renderBoardState()
            
    
    def renderMove(self, move: Move):
        if(move.is_capture() or move.is_en_passant()):
            self.setStyleSheet(CAPTURE_STYLE)
        elif(move.is_castle() or move.is_quiet()):
            self.setStyleSheet(NON_CAPTURE_STYLE)
        self.move = move

    def renderCheck(self):
        self.setStyleSheet(CHECK_STYLE)
        
    def reset(self):
        self.highlighted = False
        self.move = None
        self.setStyleSheet(TILE_STYLE.format(color = self.tileColor)) 


def isBrownTile(i, j):
    return (i + j) % 2 == 0

class Window(QtWidgets.QWidget): 
    def __init__(self):
        super().__init__()

        board.reset()
        self.boardState = board.get_board_state()

    def resizeEvent(self, event: QtGui.QResizeEvent):
        super().resizeEvent(event)
        windowSize: tuple[int] = event.size().toTuple()
        self.windowWidth: int = windowSize[0]
        self.windowHeight: int = windowSize[1]
        self.length: int = min(windowSize)


    def paintEvent(self, event: QtGui.QPaintEvent):
        topLeftWidth: int = (self.windowWidth - self.length) // 2
        topLeftHeight: int = (self.windowHeight - self.length) // 2
        tileLength: int = self.length // 8
        tileSize: QtCore.QSize = QtCore.QSize(tileLength, tileLength)

        painter: QtGui.QPainter = QtGui.QPainter(self)
        painter.setPen(BLACK_PEN)
        index: int = 63
        for j in range(0, 8):
            for i in range(0, 8):
                piece: Piece = self.boardState.piece(index)
                tileTopLeft: QtCore.QPoint = QtCore.QPoint(topLeftWidth + i * tileLength, topLeftHeight + j * tileLength)
                painter.setBrush(BROWN_BRUSH if isBrownTile(i, j) else WHITE_BRUSH)
                tileRect: QtCore.QRect = QtCore.QRect(tileTopLeft, tileSize)
                painter.drawRect(tileRect)
                ICONS[piece].render(painter, tileRect)
                index -= 1

       
    def undoLastMove(self):
        board.undo_last_move()
        self.renderBoardState()

    def printBoard(self):
        print(board.layout())


    def renderBoardState(self):
        self.boardState = board.get_board_state()
        self.selectedSquare = -1
        self.highlighted = []
        index = 63
        for i in range(0, 8):
            for j in range(0, 8):
                tile = self.tileLayout.itemAtPosition(i, j).widget()
                tile.reset()
                piece = self.boardState.piece(index)
                tile.setIcon(ICONS[piece])
                if((piece == Piece.WHITE_KING and self.boardState.in_check(Color.WHITE)) or \
                    (piece == Piece.BLACK_KING and self.boardState.in_check(Color.BLACK))):
                    tile.renderCheck()
                index -= 1
        

    def renderAttacks(self, tileNum):
        for tile in self.highlighted:
            tile.reset()
        self.highlighted = []

        moveList = self.boardState.move_list(tileNum)
        for sq in range(0, moveList.size()):
            move = moveList[sq]
            dest = move.dest()
            pos = numToPos(dest)
            tile = self.tileLayout.itemAtPosition(*pos).widget()
            self.highlighted.append(tile)
            tile.renderMove(move)
            



