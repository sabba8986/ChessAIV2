#define PY_SSIZE_T_CLEAN
#include <python3.14/Python.h>


extern void create_board();
extern char *get_board_state();


static PyObject *py_create_board(PyObject *self, PyObject *args){
    create_board();
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *py_get_board_state(PyObject *self, PyObject *args){
    char *board_c = get_board_state();
    PyObject *board_py = PyUnicode_FromString(board_c);
    free(board_c);
    return board_py;
}


static PyMethodDef board_funcs[] = {
    {"create_board", py_create_board, METH_VARARGS, "Resets the main board game by creating a new board." }, 
    {"get_board_state", py_get_board_state, METH_VARARGS, "Retrieves the state of the current board as a 64-character string, which lists spaces on the board from left to right, top to bottom."}, 
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef board_module = {
    PyModuleDef_HEAD_INIT, 
    "board", 
    NULL, 
    0, 
    board_funcs
};


PyMODINIT_FUNC PyInit_board(void){
    return PyModuleDef_Init(&board_module);
}
