from PySide6.QtWidgets import QWidget, QDialog  # noqa: I001
from PySide6.QtGui import QResizeEvent, QPaintEvent, QMouseEvent, QPainter, QShortcut, QPixmap, QImage, QColorConstants, QCursor
from PySide6.QtCore import QPoint, QRect, QSize, Qt
from PySide6.QtSvg import QSvgRenderer
from tile import Tile
from constants import TILE_PEN, SELECTED_PEN, HIGHLIGHTED_PEN, CAPTURE_BRUSH, NON_CAPTURE_BRUSH, CHECK_BRUSH, PIECE_RENDERERS, cursorTileTopLeft
import board
from board import Piece, PieceType, BoardState, MoveList, Move, Color
from promotionDialog import PromotionDialog
import time




class BoardWidget(QWidget):
    def __init__(self, parent = None):
        super().__init__(parent)
        board.reset()
        self.boardState: BoardState = board.get_board_state()
        self.boardPixmap: QPixmap = QPixmap()
        self.piecePixmaps: list[QPixmap] = [QPixmap()] * 14
        self.tileLength: int = 0
        self.length: int = 0
        self.selectedSq: int | None = None
        self.grabbed: bool = False
        self.highlighted: int = 0
        self.tiles: list[Tile] = [Tile(tileNum) for tileNum in range(64)]

        self.undoMoveShortcut = QShortcut("Ctrl+Z", self)
        self.undoMoveShortcut.activated.connect(self.undoMoveCallback)

        self.printBoardShortcut = QShortcut("Ctrl+P", self)
        self.printBoardShortcut.activated.connect(self.printBoardCallback)


    def undoMoveCallback(self) -> None:
        board.undo_last_move()
        self.boardState = board.get_board_state()
        self.update()


    def printBoardCallback(self) -> None:
        print(board.layout())


    def resizeEvent(self, event: QResizeEvent) -> None:
        boardSize: QSize = event.size()
        tileLength: int = boardSize.width() // 8 # event.size() returns the new size of the boardWidget, which will only ever be square, so obtaining only one dimension is sufficient
        tileSize: QSize = QSize(tileLength, tileLength)
        boardImage: QImage = QImage(boardSize, QImage.Format.Format_RGB32)
        boardPainter: QPainter = QPainter(boardImage)
        boardPainter.setPen(TILE_PEN)
        for tile in self.tiles:
            tile.resize(tileLength)
            boardPainter.setBrush(tile.brush)
            boardPainter.drawRect(tile.rect)
        self.boardPixmap = QPixmap.fromImage(boardImage)
        for piece in Piece:
            renderer: QSvgRenderer = PIECE_RENDERERS[piece]
            pieceImage: QImage = QImage(tileSize, QImage.Format.Format_ARGB32)
            pieceImage.fill(QColorConstants.Transparent)
            renderer.render(QPainter(pieceImage))
            self.piecePixmaps[piece] = QPixmap.fromImage(pieceImage)
        self.tileLength = tileLength
        self.length = boardSize.width()

    
    def paintEvent(self, event: QPaintEvent) -> None:
        # start: float = time.perf_counter_ns()
        painter: QPainter = QPainter(self)
        # Overlay board pixmap
        painter.drawPixmap(QPoint(0, 0), self.boardPixmap)
        for sq in range(64):
            if sq == self.selectedSq:
                continue
            piece: Piece = self.boardState.piece(sq)
            tile: Tile = self.tiles[sq]
            if (1 << sq) & self.highlighted:
                move: Move = tile.moves[0]                
                painter.setPen(HIGHLIGHTED_PEN)
                painter.setBrush(CAPTURE_BRUSH if move.is_capture() or move.is_en_passant() else NON_CAPTURE_BRUSH)
                painter.drawRect(tile.rect)
            if piece != Piece.EMPTY:
                if piece.type() == PieceType.KING and board.in_check(piece.color()):
                    painter.setPen(TILE_PEN)
                    painter.setBrush(CHECK_BRUSH)
                    painter.drawRect(tile.rect)
                painter.drawPixmap(tile.topLeft, self.piecePixmaps[self.boardState.piece(sq)])  

        if self.selectedSq is not None:
            piece: Piece = self.boardState.piece(self.selectedSq)
            tile: Tile = self.tiles[self.selectedSq]
            painter.setPen(SELECTED_PEN)
            if piece.type() == PieceType.KING and board.in_check(piece.color()):
                painter.setBrush(CHECK_BRUSH)
            else:
                painter.setBrush(Qt.BrushStyle.NoBrush)
            painter.drawRect(tile.rect)
            if piece != Piece.EMPTY:
                if not self.grabbed:
                    painter.drawPixmap(tile.topLeft, self.piecePixmaps[piece])
                else:
                    topLeft: QPoint = cursorTileTopLeft(self.mapFromGlobal(QCursor.pos()), self.tileLength)
                    painter.drawPixmap(topLeft, self.piecePixmaps[piece])
        # end: float = time.perf_counter_ns()
        # print(f"Finished paintEvent in {(end - start)/1000000} ms")


    def makeMove(self, sq: int) -> None:
        tile: Tile = self.tiles[sq] 
        #If a square is highlighted it must have at least one move
        selectedMove: Move = tile.moves[0]
        #If a tile has more than one move, it means it is a promotion
        if len(tile.moves) > 1:
            piece: Piece = self.boardState.piece(sq)
            dialog: PromotionDialog = PromotionDialog(piece.color(), tile.moves, self)
            if dialog.exec() == QDialog.DialogCode.Accepted:
                selectedMove = dialog.getSelectedMove()
        board.make_move(selectedMove)
        self.highlighted = 0
        self.selectedSq = None
        self.boardState = board.get_board_state()


    def getSelectedSq(self, point: QPoint) -> int:
        # Do not use QPoint scalar division operator, rounds up or down instead of flooring result
        j: int = (point.x() * 8) // self.length
        i: int = (point.y() * 8) // self.length
        return 63 - (8 * i + j)


    def mousePressEvent(self, event: QMouseEvent) -> None:
        sq: int = self.getSelectedSq(event.position().toPoint())
        if sq == self.selectedSq:
            return
        #If selected square is a highlighted move
        if self.highlighted & (1 << sq):
            self.makeMove(sq)
        else:
            self.selectedSq = sq
            self.highlighted = 0
            moveList: MoveList = self.boardState.move_list(sq)
            for tile in self.tiles:
                tile.moves = []
            for i in range(moveList.size()):
                move: Move = moveList[i]
                moveSq: int = move.dest()
                tile: Tile = self.tiles[moveSq]
                self.highlighted |= (1 << moveSq)
                self.tiles[moveSq].moves.append(move)
        self.grabbed = False
        self.update()


    def mouseMoveEvent(self, event: QMouseEvent):
        self.grabbed = True
        self.update()


    def mouseReleaseEvent(self, event: QMouseEvent):
        self.grabbed = False
        sq: int = self.getSelectedSq(event.position().toPoint())
        if self.highlighted & (1 << sq):
            self.makeMove(sq)
        self.update()

