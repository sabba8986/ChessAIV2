from PySide6.QtWidgets import QDialog, QLayout, QHBoxLayout, QVBoxLayout, QWidget, QPushButton, QLabel
from PySide6.QtCore import QSize, Qt
from PySide6.QtGui import QCloseEvent, QFont
from board import Color
from constants import WHITE_PROMOTION_ICONS, BLACK_PROMOTION_ICONS


class PromotionDialog(QDialog):
    def __init__(self, color: Color, parent: QWidget | None = None):
        super().__init__(parent, Qt.WindowType.FramelessWindowHint)
        self.setWindowTitle("Promotion")
        
        mainLayout: QLayout = QVBoxLayout()

        label: QLabel = QLabel("Select a promotion type below")
        font: QFont = QFont("Roboto", 16)
        label.setFont(font)
        label.setAlignment(Qt.AlignmentFlag.AlignCenter)
        mainLayout.addWidget(label)

        buttonLayout: QLayout = QHBoxLayout()
        for promotionType, icon in (WHITE_PROMOTION_ICONS if color == Color.WHITE else BLACK_PROMOTION_ICONS):
            button: QPushButton = QPushButton(icon, "", parent = self)
            button.setFixedSize(QSize(100, 100))
            button.setIconSize(QSize(100, 100))
            button.clicked.connect(lambda checked = False, p = promotionType: self.done(p))
            buttonLayout.addWidget(button)

        mainLayout.addLayout(buttonLayout)
        mainLayout.setSizeConstraint(QLayout.SizeConstraint.SetFixedSize)
        self.setLayout(mainLayout)


    def closeEvent(self, event: QCloseEvent):
        event.ignore()

            
            

