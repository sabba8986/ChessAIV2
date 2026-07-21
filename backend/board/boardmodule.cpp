#include "board.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/native_enum.h>

namespace py = pybind11;


Board current_board;


PYBIND11_MODULE(board_interface, m, py::mod_gil_not_used()){
    m.doc() = "plugin to the C++ board engine";
    py::native_enum<Color>(m, "Color", "enum.IntEnum")
        .value("WHITE", Color::WHITE)
        .value("BLACK", Color::BLACK)
        .finalize();
    py::native_enum<PieceType>(m, "PieceType", "enum.IntEnum")
        .value("PAWN", PieceType::PAWN)
        .value("ROOK", PieceType::ROOK)
        .value("KNIGHT", PieceType::KNIGHT)
        .value("BISHOP", PieceType::BISHOP)
        .value("QUEEN", PieceType::QUEEN)
        .value("KING", PieceType::KING)
        .value("EMPTY", PieceType::EMPTY)
        .finalize();
    py::native_enum<Piece>(m, "Piece", "enum.IntEnum")
        .value("EMPTY", Piece::EMPTY)
        .value("WHITE_PAWN", Piece::WHITE_PAWN)
        .value("WHITE_ROOK", Piece::WHITE_ROOK)
        .value("WHITE_KNIGHT", Piece::WHITE_KNIGHT)
        .value("WHITE_BISHOP", Piece::WHITE_BISHOP)
        .value("WHITE_QUEEN", Piece::WHITE_QUEEN)
        .value("WHITE_KING", Piece::WHITE_KING)
        .value("BLACK_PAWN", Piece::BLACK_PAWN)
        .value("BLACK_ROOK", Piece::BLACK_ROOK)
        .value("BLACK_KNIGHT", Piece::BLACK_KNIGHT)
        .value("BLACK_BISHOP", Piece::BLACK_BISHOP)
        .value("BLACK_QUEEN", Piece::BLACK_QUEEN)
        .value("BLACK_KING", Piece::BLACK_KING)
        .finalize();
    py::class_<Move>(m, "Move")
        .def(py::init<Move>())
        .def("src", &Move::src)
        .def("dest", &Move::dest)
        .def("is_capture", &Move::is_capture)
        .def("is_quiet", &Move::is_quiet)
        .def("is_castle", &Move::is_castle)
        .def("is_en_passant", &Move::is_en_passant)
        .def("is_promotion", &Move::is_promotion)
        .def("promoted_type", &Move::promoted_type)
        .def("set_promoted_type", &Move::set_promoted_type);
    py::class_<MoveList>(m, "MoveList")
        .def("size", &MoveList::size)
        .def("__getitem__", &MoveList::operator[]);
    py::class_<BoardState>(m, "BoardState")
        .def("in_check", &BoardState::in_check)
        .def("move_list", &BoardState::move_list)
        .def("piece", &BoardState::piece);
    m.def("reset", [](){current_board.reset();}, "Resets the state of the board");
    m.def("get_board_state", [](){return current_board.get_board_state();}, "Get the current state of the board");
    m.def("make_move", [](Move move){current_board.make_move(move);}, "Execute the specified move on the board");
    m.def("undo_last_move", [](){current_board.undo_last_move();}, "Undoes the last move on the board");
    m.def("layout", [](){return current_board.layout();}, "Gets the layout of the board as a prettified FEN string");
    m.def("get_color", &get_color, "Returns the color of the specified piece");
}