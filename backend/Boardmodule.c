#define PY_SSIZE_T_CLEAN
#include <python3.14/Python.h>
#include "Squaremodule.h"


extern void create_board();
extern int type_of_piece(int i);
extern bool is_white_piece(int i);


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

}


static PyObject *py_create_board(PyObject *self, PyObject *args){
    create_board();
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *py_get_board_state(PyObject *self, PyObject *args){
    PyObject *list = PyList_New(64);
    if(list == NULL){
        return NULL;
    }
    for(int i = 0; i < 64; i++){
        int type = type_of_piece(i);
        int is_white = is_white_piece(i) ? 1 : 0;
        PyObject *p = PyObject_CallFunction((PyObject*)&squareType, "ii", type, is_white);
        if(p == NULL){
            return NULL;
        }
        PyList_SET_ITEM(list, i, p);
    }
    return list;
}


static PyMethodDef board_funcs[] = {
    {"create_board", py_create_board, METH_VARARGS, "Resets the main board game by creating a new board." }, 
    {"get_board_state", py_get_board_state, METH_VARARGS, "Retrieves the state of the current board as a list of 64 squares, each containing type and color of the piece on that square."}, 
    {NULL, NULL, 0, NULL}
};


static PyModuleDef_Slot board_module_slots[] = {
    {Py_mod_exec, square_exec},
    {Py_mod_exec, board_module_exec},
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
