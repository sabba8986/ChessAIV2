from PySide6.QtWidgets import QDialog, QLayout, QHBoxLayout, QVBoxLayout, QWidget, QPushButton, QLabel
from PySide6.QtCore import QSize, Qt
from PySide6.QtGui import QCloseEvent, QFont, QIcon
from board import Color, Move
from constants import WHITE_PROMOTION_ICONS, BLACK_PROMOTION_ICONS


class PromotionDialog(QDialog):
    def __init__(self, color: Color, promotionMoves: list[Move], parent: QWidget | None = None):
        super().__init__(parent, Qt.WindowType.FramelessWindowHint)
        self.setWindowTitle("Promotion")

        self.selectedMove: Move = promotionMoves[0] # initialize to garbage move
        
        mainLayout: QLayout = QVBoxLayout()

        label: QLabel = QLabel("Select a promotion type below")
        font: QFont = QFont("Roboto", 16)
        label.setFont(font)
        label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        mainLayout.addWidget(label)

        buttonLayout: QLayout = QHBoxLayout()
        icons: list[QIcon] = WHITE_PROMOTION_ICONS if color == Color.WHITE else BLACK_PROMOTION_ICONS
        for move in promotionMoves:
            button: QPushButton = QPushButton(icons[move.promoted_type()], "", parent = self)
            button.setFixedSize(QSize(100, 100))
            button.setIconSize(QSize(100, 100))
            button.clicked.connect(lambda checked, move = move: self.buttonCallback(checked, move))
            buttonLayout.addWidget(button)

        mainLayout.addLayout(buttonLayout)
        mainLayout.setSizeConstraint(QLayout.SizeConstraint.SetFixedSize)
        self.setLayout(mainLayout)


    def buttonCallback(self, checked, move: Move) -> None:
        self.selectedMove = move
        self.accept()


    def getSelectedMove(self) -> Move:
        return self.selectedMove


    def closeEvent(self, event: QCloseEvent):
        event.ignore()

            
            

