#pragma once

#ifndef GLAD_GL_IMPLEMENTATION
#define GLAD_GL_IMPLEMENTATION
#endif

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include "objectLoader.hpp"
#include "meshHandler.hpp"
#include "camera.hpp"
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
        // void createVertexArrayObject();
        void createVertexBufferObject();
        void drawMeshObjects();
        void processInput(GLFWwindow* window, float deltaTime);
        void handleMouse();

    private:
        GLFWwindow* m_window;
        GLuint m_VBO, m_VAO;
        Shader m_shader;
        MeshHandler m_meshHandler;
        ObjectLoader m_objectLoader;
        EntityManager m_entityManager;
        Camera m_camera;

        float m_lastX = 400.0f;
        float m_lastY = 300.0f;
        bool m_firstMouse = true;

        bool m_canHandleMouse = false;

        glm::vec3 m_lightingPosition;
    };
}
