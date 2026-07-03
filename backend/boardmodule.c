#define PY_SSIZE_T_CLEAN
#include "piece.hpp"
#include <python3.14/Python.h>
#include <stdint.h>
#include "boardstatemodule.c"
const char *reps = "prnbqk_";

extern void create_board();
extern const Piece *pieces();
extern uint64_t get_legal_attacks(int sq); 
extern void make_move(int s, int d);
extern bool in_check(Color c);

static PyObject *BoardError = NULL;

static int board_module_exec(PyObject *o){
    if(BoardError != NULL){
        PyErr_SetString(PyExc_ImportError, "Reinitialization of board module");
        return -1;
    }
    BoardError = PyErr_NewException("board.error", NULL, NULL);
    if(PyModule_AddObjectRef(o, "BoardError", BoardError) < 0){
        return -1;
    }
    if(PyType_Ready(&BoardStateType) < 0){
        return -1;
    }
    if(PyModule_AddObjectRef(o, "BoardState", (PyObject*)&BoardStateType) < 0){
        return -1;
    }
    return 0;
}


static PyObject *py_create_board(PyObject *self, PyObject *args){
    create_board();
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *py_get_icon_indices(PyObject *self, PyObject *args){
    PyObject *board = PyUnicode_New(64, 127); 
    if(board == NULL) return NULL;
    const Piece *piece_list = pieces();
    for(int i = 0; i < 64; i++){
        const Piece piece_struct = piece_list[i];
        char piece = reps[piece_struct.type];
        if(piece != '_' && piece_struct.color == WHITE){
            piece -= 32; //converts to uppercase
        }
        PyUnicode_WriteChar(board, i, piece);
    }
    return board;
}

static PyObject *py_get_legal_attacks(PyObject *self, PyObject *args){
    PyObject *non_captures = PyList_New(0);
    PyObject *captures = PyList_New(0);
    if(captures == NULL || non_captures == NULL) return NULL;
    int piece_idx;
    if(!PyArg_ParseTuple(args, "i", &piece_idx)) return NULL;
    uint64_t attacks = get_legal_attacks(piece_idx);
    const Piece *piece_list = pieces();
    bool opp_color = !piece_list[piece_idx].color;
    while(attacks){
        int sq = __builtin_ctzll(attacks);
        PyObject *num = PyLong_FromLong(sq);
        if(num == NULL) return NULL;
        const Piece piece = piece_list[sq];
        if(piece.type != EMPTY && piece.color == opp_color){
            if(PyList_Append(captures, num)) return NULL;
        }
        else{
            if(PyList_Append(non_captures, num)) return NULL;
        }
        attacks &= ~(1ull << sq);
    }
    PyObject *result = PyTuple_Pack(2, non_captures, captures);
    if(result == NULL) return NULL; 
    return result;
}

static PyObject *py_make_move(PyObject *self, PyObject *args){
    int s, d;
    if(!PyArg_ParseTuple(args, "ii", &s, &d)){
        return NULL;
    }
    make_move(s, d);
    Py_RETURN_NONE;
}

static PyObject *py_in_check(PyObject *self, PyObject *args){
    int color;
    if(!PyArg_ParseTuple(args, "i", &color)){
        return NULL;
    }
    if(in_check((Color)color)){
        Py_RETURN_TRUE;
    }
    else{
        Py_RETURN_FALSE;
    }
}


static PyMethodDef board_funcs[] = {
    {"create_board", py_create_board, METH_VARARGS, "Resets the main board game by creating a new board." }, 
    {"get_board_state", py_get_icon_indices, METH_VARARGS, "Returns a string representation of the board, right to left, bottom to top."},
    {"get_legal_attacks", py_get_legal_attacks, METH_VARARGS, "Returns all possible attacks from the given square, as a list of quiet moves and a list of capture moves."},
    {"make_move", py_make_move, METH_VARARGS, "Perform a valid move on the board."},
    {"in_check", py_in_check, METH_VARARGS, "Returns whether the king of the specified color is in check."},
    {NULL, NULL, 0, NULL}
};


static PyModuleDef_Slot board_module_slots[] = {
    {Py_mod_exec, board_module_exec},
    {Py_mod_exec, board_state_exec},
    {Py_mod_multiple_interpreters, Py_MOD_MULTIPLE_INTERPRETERS_NOT_SUPPORTED},
    {0, NULL}
};

static struct PyModuleDef board_module = {
    .m_base = PyModuleDef_HEAD_INIT, 
    .m_name = "board", 
    .m_size = 0,
    .m_methods = board_funcs,
    .m_slots = board_module_slots
};


PyMODINIT_FUNC PyInit_board(void){
    return PyModuleDef_Init(&board_module);
}
