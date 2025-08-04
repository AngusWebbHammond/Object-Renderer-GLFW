#pragma once

#ifndef GLAD_GL_IMPLEMENTATION
#define GLAD_GL_IMPLEMENTATION
#endif

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "renderer.hpp"
#include "globals.hpp"

namespace ObjectRenderer {

    class Application {
    public:
        Application(const GLuint width, const GLuint height);
        ~Application();
        void run();

    private:
        void createWindow(const GLuint width, const GLuint height);

    private:
        GLFWwindow* m_window;
        Renderer m_renderer;
    };
}