from PySide6 import QtCore, QtWidgets, QtGui
import board 
from constants import ICONS, TILE_STYLE, HIGHLIGHT_STYLE, BROWN, WHITE 

class Tile(QtWidgets.QPushButton): 
    def __init__(self, window: QtWidgets.QWidget, pos: tuple[int, int]):
        super().__init__(window)
        self.tilePos = pos
        self.tileNum = (8 * pos[0]) + pos[1]
        self.tileColor = WHITE if ((pos[0] + pos[1]) % 2 == 0) else BROWN
        self.setFixedSize(QtCore.QSize(100, 100))
        self.setIconSize(QtCore.QSize(100, 100))
        self.setStyleSheet(TILE_STYLE.format(color = self.tileColor)) 
        self.setFlat(True)
        self.clicked.connect(self.onSelectTile)

    def onSelectTile(self):
        attacks = board.get_attack(self.tileNum)
        self.window().highlightSquares(attacks)
        #pass

    def reset_color(self):
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
                self.tileLayout.addWidget(tile, i, j) #inverted layout of board

        self.infoLayout = QtWidgets.QVBoxLayout()

        self.mainLayout = QtWidgets.QHBoxLayout()
        self.mainLayout.addLayout(self.tileLayout)
        self.mainLayout.addLayout(self.infoLayout)

        self.highlighted = []
        self.setLayout(self.mainLayout)

        board.create_board()

        self.renderBoardPieces(board.get_board_state())


    def renderBoardPieces(self, boardState):
        index = 63
        for i in range(0, 8):
            for j in range(0, 8):
                self.tileLayout.itemAtPosition(i, j).widget().setIcon(ICONS[boardState[index]])
                index -= 1

        

    def highlightSquares(self, squares):
        for tile in self.highlighted:
            tile.reset_color()
        self.highlighted = []
        for sq in squares:
            i = (63 - sq) // 8
            j = (63 - sq) % 8
            tile = self.tileLayout.itemAtPosition(i, j).widget()
            self.highlighted.append(tile)
            tile.setStyleSheet(HIGHLIGHT_STYLE)



