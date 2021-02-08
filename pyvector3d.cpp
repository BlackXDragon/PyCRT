#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <structmember.h>
#include "source/vector3d.h"
#include <iostream>

typedef struct {
    PyObject_HEAD
    /* Type-specific fields go here. */
	Vector3D obj_v;
} PyVector3D;

extern PyTypeObject PyVector3D_Type;

static PyObject* PyVector3D_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    PyVector3D* self;

    self = (PyVector3D*)type->tp_alloc(type, 0);
    self->obj_v = Vector3D();

    return (PyObject *)self;
}

static int PyVector3D_Init(PyVector3D *self, PyObject *args, PyObject *kwargs) {
    static char *kwlist[] = {"x", "y", "z", NULL};
    PyArg_ParseTupleAndKeywords(args, kwargs, "|ddd", kwlist, &self->obj_v.x, &self->obj_v.y, &self->obj_v.z);

    return 0;
}

static PyObject* PyVector3D_repr(PyVector3D *self) {
    std::string repr = self->obj_v.__repr__();
    return Py_BuildValue("s#", repr.c_str(), repr.length());
}

PyVector3D* make_PyVector3D(Vector3D* v) {
    PyVector3D *obj = (PyVector3D*)PyVector3D_new(&PyVector3D_Type, new PyObject(), new PyObject());
    obj->obj_v = *v;
    return obj;
}

static PyObject* PyVector3D_dot(PyVector3D* self, PyObject *args) {
    long double data;
    PyVector3D* other;
    PyObject *result;

    PyArg_ParseTuple(args, "O", &other);

    data = self->obj_v.dot(other->obj_v);

    result = Py_BuildValue("d", data);

    return result;
}

static PyObject* PyVector3D_mod(PyVector3D* self) {
    long double data;
    PyObject *result;

    data = self->obj_v.mod();

    result = Py_BuildValue("d", data);

    return result;
}

static PyVector3D* PyVector3D_normalize(PyVector3D* self) {
    Vector3D data;
    PyVector3D *result;

    data = self->obj_v.normalize();

    result = make_PyVector3D(&data);

    return result;
}

static PyVector3D* PyVector3D_add(PyVector3D* self, PyVector3D* other) {
    Vector3D data;
    PyVector3D *result;
    // std::cout << "Self: " << self->obj_v << "\n";
    // std::cout << "Other: " << other->obj_v << "\n";

    data = self->obj_v+other->obj_v;
    result = make_PyVector3D(&data);

    return result;
}

static PyVector3D* PyVector3D_subtract(PyVector3D* self, PyVector3D* other) {
    Vector3D data;
    PyVector3D *result;
    // std::cout << "Self: " << self->obj_v << "\n";
    // std::cout << "Other: " << other->obj_v << "\n";

    data = self->obj_v-other->obj_v;
    result = make_PyVector3D(&data);

    return result;
}

static PyVector3D* PyVector3D_multiply(PyVector3D* self, PyObject* args) {
    Vector3D data;
    PyVector3D *result;
    // std::cout << "Self: " << self->obj_v << "\n";
    // std::cout << "Other: " << other->obj_v << "\n";

    // std::cout << "other type: " << PyNumber_Check(&args[0]) << "\n";
    // std::cout << "other type: " << ((PyTypeObject*)PyObject_Type(&args[0]))->tp_name << "\n";
    ((PyTypeObject*)PyObject_Type(&args[0]))->tp_name;

    long double scalar;
    scalar = PyFloat_AsDouble(&args[0]);
    // std::cout << "Scalar: " << scalar << "\n";

    data = self->obj_v*scalar;

    // std::cout << "Result is: " << data << '\n';

    result = make_PyVector3D(&data);

    return result;
}

static PyNumberMethods PyVector3D_num_methods[] = {
    (binaryfunc)PyVector3D_add,         //nb_add
    (binaryfunc)PyVector3D_subtract,    //nb_subtract
    (binaryfunc)PyVector3D_multiply,    //nb_multiply
};

static PyMemberDef PyVector3D_members[] = {
    {"x", T_DOUBLE, offsetof(PyVector3D, obj_v), 0, "Vector3D x"},
    {"y", T_DOUBLE, offsetof(PyVector3D, obj_v)+sizeof(long double), 0, "Vector3D y"},
    {"z", T_DOUBLE, offsetof(PyVector3D, obj_v)+(2*sizeof(long double)), 0, "Vector3D z"},
    {NULL}  /* Sentinel */
};

static PyMethodDef PyVector3D_methods[] = {
    {"mod", (PyCFunction)PyVector3D_mod, METH_NOARGS,
     "Return the absolute value/distance of the vector from origin.",
    },
    {"dot", (PyCFunction)PyVector3D_dot, METH_O,
     "Return dot product of self and other.",
    },
    {"normalize", (PyCFunction)PyVector3D_normalize, METH_NOARGS,
     "Normalize and return the vector.",
    },
    {NULL}  /* Sentinel */
};

static PyTypeObject PyVector3D_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)  //PyObject_VAR_HEAD
    "crt.Vector3D",                 //tp_name
    sizeof(PyVector3D),             //tp_basicsize
    0,                              //tp_itemsize
    0,                              //tp_dealloc
    0,                              //tp_vectorcall_offset
    0,                              //tp_getattr
    0,                              //tp_setattr
    0,                              //tp_as_async
    (reprfunc)PyVector3D_repr,                //tp_repr
    PyVector3D_num_methods,         //tp_as_number
    0,                              //tp_as_sequence
    0,                              //tp_as_mapping
    0,                              //tp_hash
    0,                              //tp_call
    0,                              //tp_str
    0,                              //tp_getattro
    0,                              //tp_setattro
    0,                              //tp_as_buffer
    Py_TPFLAGS_DEFAULT,             //tp_flags
    "3D Vector type",               //tp_doc
    0,                              //tp_traverse
    0,                              //tp_clear
    0,                              //tp_richcompare
    0,                              //tp_weaklistoffset
    0,                              //tp_iter
    0,                              //tp_iternext
    PyVector3D_methods,             //tp_methods
    PyVector3D_members,             //tp_members
    0,                              //tp_getset
    0,                              //tp_base
    0,                              //tp_dict
    0,                              //tp_descr_get
    0,                              //tp_descr_set
    0,                              //tp_dictoffset
    (initproc)PyVector3D_Init,      //tp_init
    0,                              //tp_alloc
    PyVector3D_new,              //tp_new
};

static PyModuleDef crtmodule = {
    PyModuleDef_HEAD_INIT,
    "crt",                  //m_name
    "Ray Tracing in C++",   //m_doc
    -1,                     //m_size
};

PyMODINIT_FUNC
PyInit_crt(void)
{
    PyObject *m;
    if (PyType_Ready(&PyVector3D_Type) < 0)
        return NULL;

    m = PyModule_Create(&crtmodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&PyVector3D_Type);
    if (PyModule_AddObject(m, "Vector3D", (PyObject *) &PyVector3D_Type) < 0) {
        Py_DECREF(&PyVector3D_Type);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}