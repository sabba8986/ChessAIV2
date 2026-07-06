#include "boardmodule.hpp"
#include "board.hpp"

namespace py = pybind11;


Board current_board;


void reset(){
    current_board.reset();
}

BoardState get_board_state(){
    return current_board.get_board_state();
}


PYBIND11_MODULE(board_interface, m, py::mod_gil_not_used()){
    m.doc() = "plugin to the C++ board engine";
    py::enum_<Color>(m, "Color")
        .value("WHITE", Color::WHITE, "Represents a white piece.")
        .value("BLACK", Color::BLACK, "Represents a black piece.");
    py::enum_<PieceType>(m, "PieceType")
        .value("PAWN", PieceType::PAWN, "Represents a pawn.")
        .value("ROOK", PieceType::ROOK, "Represents a rook.")
        .value("KNIGHT", PieceType::KNIGHT, "Represents a knight")
        .value("BISHOP", PieceType::BISHOP, "Represents a bishop.")
        .value("QUEEN", PieceType::QUEEN, "Represents a queen")
        .value("KING", PieceType::KING, "Represents a king.")
        .value("EMPTY", PieceType::EMPTY, "Represents no piece.");
    py::class_<Piece>(m, "Piece")
        .def(py::init(
            [](Color c, PieceType p)-> Piece{
                return {c, p};
            }))
        .def("__eq__", 
            [](Piece a, Piece b){
                return (a.type == EMPTY && b.type == EMPTY) || (a.type == b.type && a.color == b.color);
            })
        .def_readwrite("color", &Piece::color)
        .def_readwrite("type", &Piece::type);
    py::class_<Move>(m, "Move")
        .def("src", &Move::src)
        .def("dest", &Move::dest)
        .def("is_capture", &Move::is_capture)
        .def("is_quiet", &Move::is_quiet)
        .def("is_castle", &Move::is_castle)
        .def("is_en_passant", &Move::is_en_passant)
        .def("is_promotion", &Move::is_promotion)
        .def("promoted_type", &Move::promoted_type);
    py::class_<MoveList>(m, "MoveList")
        .def("size", &MoveList::size)
        .def("__getitem__", &MoveList::operator[]);
    py::class_<BoardState>(m, "BoardState")
        .def("in_check", &BoardState::in_check)
        .def("move_list", &BoardState::move_list)
        .def("piece", &BoardState::piece);
    m.def("reset", &reset, "Resets the state of the board");
    m.def("get_board_state", &get_board_state, "Get the current state of the board");
}