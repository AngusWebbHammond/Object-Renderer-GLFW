#pragma once

#ifndef GLAD_GL_IMPLEMENTATION
#define GLAD_GL_IMPLEMENTATION
#endif

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include "objectLoader.hpp"
#include "meshHandler.hpp"

namespace ObjectRenderer {
    class Renderer {
    public:
        Renderer();
        ~Renderer();

        void init(GLFWwindow* window);

        void run();
        void renderCycle();

    private:
        void createVertexArrayObject();
        void createVertexBufferObject();

    private:
        GLFWwindow* m_window;
        GLuint m_VBO, m_VAO;
        Shader m_shader;

        int m_verticiesLength;
        int m_texturesLength;
        int m_normalsLength;

        int m_edgesLength;
        int m_textureEdgesLength;
        int m_normalEdgesLength;

        int m_trianglesNumber;

        unsigned int m_EBO;
        ObjectLoader m_loader;
        MeshHandler m_meshHandler;
    };
}

