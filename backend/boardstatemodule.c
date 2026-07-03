#include <python3.14/Python.h>
#include "board_state.hpp"

typedef struct{
    PyObject_HEAD
    struct BoardState state;
} BoardStateObject;



static PyObject *BoardState_init

static PyTypeObject BoardStateType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "board.BoardState()",
    .tp_doc = PyDoc_STR("Board state object"),
    .tp_basicsize = sizeof(BoardStateObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew
};
