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
    int type;
    int is_white;
    if(!PyArg_ParseTuple(args, "ii", &type, &is_white)){
        return -1;
    }
    square->type = type;
    square->is_white = is_white ? true : false;
    return 0;
}


static PyMemberDef Square_members[] = {
    {"type", Py_T_INT, offsetof(SquareObject, type), 0, "Type of the chess piece on this square, represented as an integer. 0 is empty (no piece), 1 is pawn, 2 is rook, 3 is knight, 4 is bishop, 5 is queen, 6 is king"}, 
    {"is_white", Py_T_BOOL, offsetof(SquareObject, is_white), 0, "Whether the piece on this square is white. Not relevant when type is 0." }, 
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

