#pragma once

#ifndef GLAD_GL_IMPLEMENTATION
#define GLAD_GL_IMPLEMENTATION
#endif

#include <glad/gl.h>
#include <GLFW/glfw3.h>


namespace ObjectRenderer {
    const GLuint g_width = 800;
    const GLuint g_height = 800;

    void g_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
}