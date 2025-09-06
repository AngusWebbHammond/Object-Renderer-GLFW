#pragma once

#include <iostream>
#include <Python.h>

#include "Core/application.h"
#include "Core/globals.h"

int main(void)
{
#if 1
    ObjectRenderer::Application application(ObjectRenderer::g_width, ObjectRenderer::g_height);
    application.run();

#else
    // Testing
    Py_Initialize();

    if (!Py_IsInitialized()) {
        std::cerr << "Failed to initialize Python!" << std::endl;
        return 1;
    }

    PyRun_SimpleString("print('Hello World!')");

    Py_Finalize();
#endif
    return 0;
}
