from PySide6 import QtCore, QtWidgets, QtGui
import board 
from constants import PIECES, TILE_STYLE, BROWN, WHITE

class Tile(QtWidgets.QPushButton): 
    def __init__(self, window: QtWidgets.QWidget, pos: tuple[int, int]):
        super().__init__(window)
        self.tilePos = pos
        self.setFixedSize(QtCore.QSize(100, 100))
        self.setIconSize(QtCore.QSize(100, 100))
        self.setStyleSheet(TILE_STYLE.format(color = WHITE if (pos[0] + pos[1]) % 2 == 0 else BROWN)) 
        self.setFlat(True)
        self.clicked.connect(self.onSelectTile)

    def onSelectTile(self):
        print(self.tilePos)

class Window(QtWidgets.QWidget): 
    def __init__(self):
        super().__init__()


        self.tileLayout = QtWidgets.QGridLayout()
        self.tileLayout.setVerticalSpacing(0)
        self.tileLayout.setHorizontalSpacing(0)
 
        for i in range(0, 8):
            for j in range(0, 8):
                tile = Tile(self, (i, j)) 
                self.tileLayout.addWidget(tile, i, j)

        self.infoLayout = QtWidgets.QVBoxLayout()

        self.mainLayout = QtWidgets.QHBoxLayout()
        self.mainLayout.addLayout(self.tileLayout)
        self.mainLayout.addLayout(self.infoLayout)


        self.setLayout(self.mainLayout)

        board.create_board()

        self.renderBoardPieces(board.get_board_state())


    def renderBoardPieces(self, boardState: str): 
        index = 0
        for i in range(7, -1, -1):
            for j in range(7, -1, -1):
                piece = boardState[index]
                tile = self.tileLayout.itemAtPosition(i, j).widget()
                if(piece.type == -1):
                    tile.setIcon(QtGui.QIcon())
                else:
                    tile.setIcon(PIECES[1 if piece.is_white else 0][piece.type])  
                index += 1
     

                



