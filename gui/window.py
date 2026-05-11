from PySide6 import QtCore, QtWidgets, QtGui
import board 

PIECE_TO_ICON = None

BROWN = "#B88B4A"
WHITE = "#E3C16F"

TILE_STYLE = "background-color:{color}; border: black; border-width: thin"


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
        self.PIECE_TO_ICON = {
            'k': QtGui.QIcon("../assets/icons/pieces/black_king.svg"), 
            'q': QtGui.QIcon("../assets/icons/pieces/black_queen.svg"),
            'r': QtGui.QIcon("../assets/icons/pieces/black_rook.svg"), 
            'b': QtGui.QIcon("../assets/icons/pieces/black_bishop.svg"),
            'p': QtGui.QIcon("../assets/icons/pieces/black_pawn.svg"), 
            'n': QtGui.QIcon("../assets/icons/pieces/black_knight.svg"), 

            'K': QtGui.QIcon("../assets/icons/pieces/white_king.svg"), 
            'Q': QtGui.QIcon("../assets/icons/pieces/white_queen.svg"), 
            'R': QtGui.QIcon("../assets/icons/pieces/white_rook.svg"), 
            'B': QtGui.QIcon("../assets/icons/pieces/white_bishop.svg"), 
            'P': QtGui.QIcon("../assets/icons/pieces/white_pawn.svg"), 
            'N': QtGui.QIcon("../assets/icons/pieces/white_knight.svg"), 
            '_': QtGui.QIcon()
        }



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
        for i in range(0, 8):
            for j in range(0, 8):
                piece = boardState[index]
                tile = self.tileLayout.itemAtPosition(i, j).widget()
                tile.setIcon(self.PIECE_TO_ICON[piece]) 
                
                index += 1
     

                



