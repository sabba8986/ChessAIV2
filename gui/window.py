from PySide6 import QtCore, QtWidgets, QtGui

BROWN = "#B88B4A"
WHITE = "#E3C16F"

TILE_STYLE = "background-color:{color}; border: black; border-width: thin"

TILESIZEPOLICY = QtWidgets.QSizePolicy()
TILESIZEPOLICY.setHorizontalPolicy(QtWidgets.QSizePolicy.Policy.Expanding)
TILESIZEPOLICY.setVerticalPolicy(QtWidgets.QSizePolicy.Policy.Expanding)


class Tile(QtWidgets.QPushButton): 
    def __init__(self, window: QtWidgets.QWidget, pos: tuple[int, int]):
        super().__init__(window)
        self.tilePos = pos
        self.setSizePolicy(TILESIZEPOLICY)
        self.setStyleSheet(TILE_STYLE.format(color = WHITE if (pos[0] + pos[1]) % 2 == 0 else BROWN)) 
        self.setFlat(True)
        self.clicked.connect(self.onSelectTile)

    def onSelectTile(self):
        print(self.tilePos)


class Window(QtWidgets.QWidget):
    def __init__(self):
        super().__init__() 
        tileLayout = QtWidgets.QGridLayout()
        tileLayout.setVerticalSpacing(0)
        tileLayout.setHorizontalSpacing(0)
 
        for i in range(0, 9):
            for j in range(0, 9):
                tile = Tile(self, (i, j)) 
                tileLayout.addWidget(tile, i, j)

        infoLayout = QtWidgets.QVBoxLayout()

        mainLayout = QtWidgets.QHBoxLayout()
        mainLayout.addLayout(tileLayout, stretch = 5)
        mainLayout.addLayout(infoLayout, stretch = 3)


        self.setLayout(mainLayout)


