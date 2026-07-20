from window import Window
import sys
from PySide6 import QtWidgets
from constants import initializePieceRenderers



if __name__ == "__main__":
    app = QtWidgets.QApplication([])
    initializePieceRenderers()
    window = Window()
    window.setFixedSize(1000, 800)
    window.show()

    sys.exit(app.exec())
