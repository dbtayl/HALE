#ifndef HALE_PYTHON_WRAPPER_H
#define HALE_PYTHON_WRAPPER_H

//Include at the top since it may well define HALE_ENABLE_PYTHON
#include "config.h"

#ifdef HALE_ENABLE_PYTHON

#include <Python.h>

#include "state.h"

PyObject* gsToPy(GameState_t* gs);

#endif //HALE_ENABLE_PYTHON

#endif //HALE_PYTHON_WRAPPER_H
