#pragma once

#ifndef GLAD_GL_IMPLEMENTATION
#define GLAD_GL_IMPLEMENTATION
#endif

#include <glad/gl.h>
#include <GLFW/glfw3.h>


namespace ObjectRenderer {
    const GLuint g_width = 800;
    const GLuint g_height = 800;

    const float g_yaw = -90.0f;
    const float g_pitch = 0.0f;
    const float g_speed = 2.5f;
    const float g_sensitivity = 0.1f;

    void g_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
}