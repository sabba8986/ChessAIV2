#define PY_SSIZE_T_CLEAN
#include <python3.14/Python.h>
#include <stddef.h>


typedef struct{
    PyObject_HEAD 
    int type;
    bool is_white;
} SquareObject;



static void Square_dealloc(PyObject *o){
    SquareObject *self = (SquareObject*)o;
    Py_TYPE(self)->tp_free(self);
}


static PyObject *Square_new(PyTypeObject *type, PyObject *args, PyObject *kwds){
    SquareObject *square;
    square = (SquareObject*)type->tp_alloc(type, 0);

    return (PyObject*)square;
}


static int Square_init(PyObject* o, PyObject *args, PyObject *kwds){
    SquareObject *square = (SquareObject*)o;
    square->type = -1;
    square->is_white = false;
    return 0;
}


static PyMemberDef Square_members[] = {
    {"type", Py_T_INT, offsetof(SquareObject, type), 0, "Type of the chess piece on this square, represented as an integer. -1 is empty (no piece), 0 is pawn, 1 is rook, 2 is knight, 3 is bishop, 4 is queen, 5 is king"}, 
    {"is_white", Py_T_BOOL, offsetof(SquareObject, is_white), 0, "Whether the piece on this square is white. Not relevant when type is -1." }, 
    {NULL}
};


static PyTypeObject squareType = {
    .ob_base = PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "square.Square", 
    .tp_doc = PyDoc_STR("Chess square object"), 
    .tp_basicsize = sizeof(SquareObject),
    .tp_itemsize = 0, 
    .tp_flags = Py_TPFLAGS_DEFAULT, 
    .tp_new = Square_new,
    .tp_init = Square_init,
    .tp_dealloc = Square_dealloc,
    .tp_members = Square_members
};


static int square_exec(PyObject *m){
    if(PyType_Ready(&squareType) < 0){
        return -1;
    }
    if(PyModule_AddObjectRef(m, "Square", (PyObject *)(&squareType)) < 0){
        return -1;
    }
    return 0;
}

