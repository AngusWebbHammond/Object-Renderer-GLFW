#pragma once

#include "application.hpp"

#include "../UI/ui.hpp"

// Public
namespace ObjectRenderer {

    Application::Application(const GLuint width, const GLuint height)
    {
        createWindow(width, height);
        m_renderer.init(m_window);
        UI::initImGui(m_window);
    }

    Application::~Application()
    {
        glfwTerminate();
        UI::cleanupImGui();
    }

    void Application::run() {
        while (!glfwWindowShouldClose(m_window))
        {
            m_renderer.renderCycle();
        }
    }
}

// Private
namespace ObjectRenderer {

    void Application::createWindow(const GLuint width, const GLuint height) {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(width, height, "[glad] GL with GLFW", NULL, NULL);
        glfwMakeContextCurrent(m_window);

        gladLoadGL(glfwGetProcAddress);
        glViewport(0, 0, width, height);
        glfwSetKeyCallback(m_window, g_keyCallback);
    }
}
