from PySide6 import QtCore, QtWidgets, QtGui
import board
from board import Piece, Color, Move
from constants import ICONS, TILE_STYLE, CAPTURE_STYLE, NON_CAPTURE_STYLE, CHECK_STYLE, BROWN, WHITE


def numToPos(sq):
    return ((63 - sq) // 8, (63 - sq) % 8)


class Tile(QtWidgets.QPushButton): 
    def __init__(self, window: QtWidgets.QWidget, pos: tuple[int, int]):
        super().__init__(window)
        self.move = None
        self.tilePos = pos
        self.tileNum = (8 * pos[0]) + pos[1]
        self.tileColor = WHITE if ((pos[0] + pos[1]) % 2 == 0) else BROWN
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



class Window(QtWidgets.QWidget): 
    def __init__(self):
        super().__init__()


        self.tileLayout = QtWidgets.QGridLayout()
        self.tileLayout.setVerticalSpacing(0)
        self.tileLayout.setHorizontalSpacing(0)
 
        for i in range(0, 8):
            for j in range(0, 8):
                tile = Tile(self, (7 - i, 7 - j)) 
                self.tileLayout.addWidget(tile, i, j) 

        self.infoLayout = QtWidgets.QVBoxLayout()

        self.mainLayout = QtWidgets.QHBoxLayout()
        self.mainLayout.addLayout(self.tileLayout)
        self.mainLayout.addLayout(self.infoLayout)

        self.highlighted = []
        self.selectedSquare = -1
        self.setLayout(self.mainLayout)

        self.undoShortcut = QtGui.QShortcut(QtGui.QKeySequence("Ctrl+Z"), self)
        self.undoShortcut.activated.connect(self.undoLastMove)

        self.printShortcut = QtGui.QShortcut(QtGui.QKeySequence("Ctrl+P"), self)
        self.printShortcut.activated.connect(self.printBoard)

        board.reset()
        self.renderBoardState()

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
            



