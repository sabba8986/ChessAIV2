from window import Window
import sys
from PySide6 import QtWidgets
from PySide6.QtCore import QSize
from constants import initializePieceRenderers



if __name__ == "__main__":
    app = QtWidgets.QApplication([])
    initializePieceRenderers()
    window = Window()
    window.resize(QSize(1000, 800))
    window.show()

    sys.exit(app.exec())
