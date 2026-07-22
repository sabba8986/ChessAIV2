from PySide6.QtWidgets import QWidget, QDialog
from PySide6.QtGui import QResizeEvent, QPaintEvent, QMouseEvent, QPainter, QPen, QBrush, QShortcut
from PySide6.QtCore import QPoint, QRect, QSize
from tile import Tile
from constants import TILE_PEN, SELECTED_PEN, HIGHLIGHTED_PEN, CAPTURE_BRUSH, NON_CAPTURE_BRUSH, CHECK_BRUSH, PIECE_RENDERERS
import board
from board import Piece, PieceType, BoardState, MoveList, Move, Color, get_type, get_color
from promotionDialog import PromotionDialog



class BoardWidget(QWidget):
    def __init__(self, parent = None):
        super().__init__(parent)
        board.reset()
        self.boardState: BoardState = board.get_board_state()
        self.selectedSq: int | None = None
        self.tiles: list[Tile] = [Tile(tileNum) for tileNum in range(0, 64)]

        self.undoMoveShortcut = QShortcut("Ctrl+Z", self)
        self.undoMoveShortcut.activated.connect(self.undoMoveCallback)

        self.printBoardShortcut = QShortcut("Ctrl+P", self)
        self.printBoardShortcut.activated.connect(self.printBoardCallback)


    def undoMoveCallback(self) -> None:
        board.undo_last_move()
        self.updateBoardState()


    def printBoardCallback(self) -> None:
        print(board.layout())

    
    def resizeEvent(self, event: QResizeEvent) -> None:
        self.length: int = event.size().width() # event.size() returns the new size of the boardWidget, which will only ever be square, so obtaining only one dimension is sufficient
        tileLength: int = self.length // 8
        tileSize: QSize = QSize(tileLength, tileLength)
        for i in range(0, 8):
            for j in range(0, 8):
                tileNum: int = 63 - (8 * i + j)
                tileTopLeft: QPoint = QPoint(j * tileLength, i * tileLength)
                self.tiles[tileNum].rect = QRect(tileTopLeft, tileSize)


    def paintTile(self, tile: Tile, piece: Piece, painter: QPainter, pen: QPen, brush: QBrush) -> None:
        painter.setPen(pen)
        painter.setBrush(brush)
        painter.drawRect(tile.rect)
        PIECE_RENDERERS[piece].render(painter, tile.rect)

    
    def paintEvent(self, event: QPaintEvent) -> None:
        painter: QPainter = QPainter(self)
        for sq in range(0, 64):
            tile: Tile = self.tiles[sq]
            piece: Piece = self.boardState.piece(sq)
            if get_type(piece) == PieceType.KING and board.in_check(get_color(piece)):
                self.paintTile(tile, piece, painter, SELECTED_PEN, CHECK_BRUSH)
            else:
                self.paintTile(tile, self.boardState.piece(sq), painter, TILE_PEN, tile.brush)

        if self.selectedSq is not None: 
            moveList: MoveList = self.boardState.move_list(self.selectedSq)
            for i in range(0, moveList.size()):
                move: Move = moveList[i]
                sq: int = move.dest()
                tile: Tile = self.tiles[sq]
                tile.moves.append(move)
                if move.is_capture() or move.is_en_passant():
                    self.paintTile(tile, self.boardState.piece(sq), painter, HIGHLIGHTED_PEN, CAPTURE_BRUSH)
                else:
                    self.paintTile(tile, self.boardState.piece(sq), painter, HIGHLIGHTED_PEN, NON_CAPTURE_BRUSH)
            # Paint selected tile last to prevent borders of highlighted squares from overriding the selected pen 
            selectedTile: Tile = self.tiles[self.selectedSq]
            self.paintTile(selectedTile, self.boardState.piece(self.selectedSq), painter, SELECTED_PEN, CHECK_BRUSH if get_type(self.boardState.piece(self.selectedSq)) == PieceType.KING else tile.brush)


    def getSelectedSq(self, point: QPoint) -> int:
        # Do not use QPoint scalar division operator, rounds up or down instead of flooring result
        j: int = (point.x() * 8) // self.length
        i: int = (point.y() * 8) // self.length
        return 63 - (8 * i + j)


    def updateBoardState(self) -> None:
        self.boardState = board.get_board_state()
        self.update()

    
    def mousePressEvent(self, event: QMouseEvent) -> None:
        newSq: int = self.getSelectedSq(event.pos())
        selectedMoves: list[Move] = self.tiles[newSq].moves
        if self.selectedSq == newSq:
            return
        for tile in self.tiles:
            tile.moves = []
        if len(selectedMoves) != 0:
            selectedMove: Move = selectedMoves[0]
            if selectedMove.is_promotion():
                color: Color = Color.WHITE if self.boardState.piece(selectedMove.src()) == Piece.WHITE_PAWN else Piece.BLACK_PAWN
                dialog: PromotionDialog = PromotionDialog(color, selectedMoves, self)
                if dialog.exec() == QDialog.DialogCode.Accepted:
                    selectedMove = dialog.getSelectedMove()
            board.make_move(selectedMove)
            self.selectedSq = None
        else:
            self.selectedSq = newSq
        event.accept()
        self.updateBoardState()