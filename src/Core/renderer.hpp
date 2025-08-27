#pragma once

#ifndef GLAD_GL_IMPLEMENTATION
#define GLAD_GL_IMPLEMENTATION
#endif

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "objectLoader.hpp"
#include "../Shader/shader.hpp"
#include "../Entity/meshHandler.hpp"
#include "../Camera/camera.hpp"
#include "../Scene/scene.hpp"

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
        void processInput(GLFWwindow* window, float deltaTime);
        void processMouse();
        void handleMouse();
        void initFrameBuffer();
        void resizeFramebuffer(int width, int height);

    private:
        GLFWwindow* m_window;
        GLuint m_VBO, m_VAO;
        Shader m_shader;
        MeshHandler m_meshHandler;
        ObjectLoader m_objectLoader;
        Scene m_scene;

        GLuint m_FBO, m_colorTexture, m_depthBuffer;
        std::vector<std::string> m_keys;

        float m_lastX = 0;
        float m_lastY = 0;
        bool m_firstMouse = true;

        int m_lastFbWidth = -1;
        int m_lastFbHeight = -1;

        bool m_canHandleMouse = false;

        glm::vec3 m_lightingPosition;
    };
}
