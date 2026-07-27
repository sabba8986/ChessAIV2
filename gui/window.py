from PySide6 import QtWidgets, QtGui
from boardWidget import BoardWidget


class Window(QtWidgets.QWidget): 
    def __init__(self):
        super().__init__()
        self.boardWidget: BoardWidget = BoardWidget(self)


    def resizeEvent(self, event: QtGui.QResizeEvent):
        super().resizeEvent(event)
        windowWidth: int
        windowHeight: int
        windowWidth, windowHeight = event.size().toTuple()
        boardLength: int = min(windowWidth, windowHeight)
        topLeftX: int = (windowWidth - boardLength) // 2
        topLeftY: int = (windowHeight - boardLength) // 2
        self.boardWidget.setGeometry(topLeftX, topLeftY, boardLength, boardLength)