#include "board.hpp"
#include <pybind11/pybind11.h>
#include <pybind11/native_enum.h>
#include <pybind11/pytypes.h>
#include <expected>
#include "fen_error.hpp"
#include <iostream>


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
    auto piece_enum = m.attr("Piece");
    piece_enum.attr("color") = py::cpp_function(
        [](Piece self){return get_color(self);}, 
        py::is_method(piece_enum)
    );
    piece_enum.attr("type") = py::cpp_function(
        [](Piece self){return get_type(self);}, 
        py::is_method(piece_enum)
    );
    py::native_enum<GameState>(m, "GameState", "enum.IntEnum")
        .value("ONGOING", GameState::ONGOING)
        .value("CHECKMATE", GameState::CHECKMATE)
        .value("STALEMATE", GameState::STALEMATE)
        .value("DRAW_HUNDRED_MOVE_CLOCK", GameState::DRAW_HUNDRED_MOVE_CLOCK)
        .value("DRAW_LACK_MATERIAL", GameState::DRAW_LACK_MATERIAL)
        .value("DRAW_REPEATED_POSITION", GameState::DRAW_REPEATED_POSITION)
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
        .def("promoted_type", &Move::promoted_type);
    py::class_<MoveList>(m, "MoveList")
        .def("size", &MoveList::size)
        .def("__getitem__", &MoveList::operator[]);
    py::class_<BoardState>(m, "BoardState")
        .def("in_check", &BoardState::in_check)
        .def("move_list", &BoardState::move_list)
        .def("piece", &BoardState::piece)
        .def("game_state", &BoardState::game_state);
    m.def("reset", [](){current_board.reset();}, "Resets the state of the board");
    m.def("get_board_state", [](){return current_board.get_board_state();}, "Get the current state of the board");
    m.def("make_move", [](Move move){
        current_board.make_move(move);
        if(current_board.get_cur_ply() > Board::get_max_plys()){
            std::cerr << "WARNING: Move stack is full, undo move operation will now be disabled for the rest of the game" << std::endl;
        }}, "Execute the specified move on the board, printing a warning if the move stack has reached capacity");
    m.def("undo_last_move", [](){
        if(current_board.get_cur_ply() == 0){
            std::cerr << "ERROR: Empty move stack, aborting undo move operation" << std::endl;
        } 
        else if(current_board.get_cur_ply() > Board::get_max_plys()){
            std::cerr << "ERROR: Move stack stopped tracking moves after ply" << Board::get_max_plys() << ", aborting undo move operation" << std::endl;
        }
        current_board.undo_last_move();
    }, "Undoes the last move on the board, printing an error if the undo operation cannot be fulfilled.");
    m.def("in_check", [](Color c){return current_board.in_check(c);}, "Returns whether the king of the specified color is in check");
    m.def("layout", [](){return current_board.layout();}, "Gets the layout of the board as a prettified FEN string");
    m.def("loadFEN", [](const std::string& str){
        auto status = current_board.load_FEN(str);
        if(!status.has_value()) std::cerr << "FEN Parsing Error: " << string_rep(status.error()) << std::endl;
    }, "Loads the specified FEN string into the board. If the FEN string is invalid, prints the resulting parsing error.");
}