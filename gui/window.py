from PySide6 import QtCore, QtWidgets, QtGui
import board 
from constants import ICONS, TILE_STYLE, HIGHLIGHT_STYLE, BROWN, WHITE 

def numToPos(sq):
    return ((63 - sq) // 8, (63 - sq) % 8)


class Tile(QtWidgets.QPushButton): 
    def __init__(self, window: QtWidgets.QWidget, pos: tuple[int, int]):
        super().__init__(window)
        self.highlighted = False
        self.tilePos = pos
        self.tileNum = (8 * pos[0]) + pos[1]
        self.tileColor = WHITE if ((pos[0] + pos[1]) % 2 == 0) else BROWN
        self.setFixedSize(QtCore.QSize(100, 100))
        self.setIconSize(QtCore.QSize(100, 100))
        self.setStyleSheet(TILE_STYLE.format(color = self.tileColor)) 
        self.setFlat(True)
        self.clicked.connect(self.onSelectTile)

    def onSelectTile(self):
        if self.highlighted:
            board.make_move(self.window().selectedSquare, self.tileNum)
            self.window().renderBoardState()
        else:
            self.window().selectedSquare = self.tileNum
            attacks = board.get_attack(self.tileNum)
            self.window().renderAttacks(self.tileNum)
    
    def highlight(self):
        self.highlighted = True
        self.setStyleSheet(HIGHLIGHT_STYLE)


    def reset(self):
        self.highlighted = False
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

        board.create_board()

        self.renderBoardState()


    def renderBoardState(self):
        index = 63
        boardState = board.get_board_state()
        for i in range(0, 8):
            for j in range(0, 8):
                tile = self.tileLayout.itemAtPosition(i, j).widget()
                tile.reset()
                tile.setIcon(ICONS[boardState[index]])
                index -= 1

        

    def renderAttacks(self, tileNum):
        for tile in self.highlighted:
            tile.reset()
        self.highlighted = []
        attacks = board.get_attack(tileNum)
        for sq in attacks:
            i = (63 - sq) // 8
            j = (63 - sq) % 8
            tile = self.tileLayout.itemAtPosition(i, j).widget()
            self.highlighted.append(tile)
            tile.highlight()



