#pragma once

#ifndef GLAD_GL_IMPLEMENTATION
#define GLAD_GL_IMPLEMENTATION
#endif

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include "objectLoader.hpp"
#include "meshHandler.hpp"
#include "entityManagementSystem.hpp"

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
        MeshHandler m_meshHandler;
        ObjectLoader m_objectLoader;
        EntityManager m_entityManager;

        int m_trianglesNumber;

        glm::vec3 m_lightingPosition;
    };
}

