#define PY_SSIZE_T_CLEAN
#include <python3.14/Python.h>
#include <stdint.h>
const char *reps = "_prnbqk";

extern void create_board();
extern int type_of_piece(int i);
extern bool is_white_piece(int i);
extern uint64_t get_attack(int sq); 

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
    return 0;
}


static PyObject *py_create_board(PyObject *self, PyObject *args){
    create_board();
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *py_get_icon_indices(PyObject *self, PyObject *args){
    PyObject *board = PyUnicode_New(64, 127); 
    if(board == NULL){
        return NULL;
    }
    for(int i = 0; i < 64; i++){
        char piece = reps[type_of_piece(i)];
        if(piece != '_' && is_white_piece(i)){
            piece -= 32; //converts to uppercase
        }
        PyUnicode_WriteChar(board, i, piece);
    }
    return board;
}

static PyObject *py_get_attack(PyObject *self, PyObject *args){
    PyObject *list = PyList_New(0);
    int p;
    if(!PyArg_ParseTuple(args, "i", &p)){
        return NULL;
    }
    uint64_t attacks = get_attack(p); 
    while(attacks){
        int sq = __builtin_ctzll(attacks);
        PyObject *num = PyLong_FromLong(sq);
        if(num == NULL){
            return NULL;
        }
        if(PyList_Append(list, num)){
            return NULL;
        }
        attacks &= ~(1ull << sq);
    }
    return list;
}

static PyMethodDef board_funcs[] = {
    {"create_board", py_create_board, METH_VARARGS, "Resets the main board game by creating a new board." }, 
    {"get_board_state", py_get_icon_indices, METH_VARARGS, "Returns a string representation of the board, right to left, bottom to top."},
    {"get_attack", py_get_attack, METH_VARARGS, "Returns a list of all possible attacks from the given square."},
    {NULL, NULL, 0, NULL}
};


static PyModuleDef_Slot board_module_slots[] = {
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
