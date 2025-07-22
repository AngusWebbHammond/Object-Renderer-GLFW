#pragma once
#include "renderer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>

// Public
namespace ObjectRenderer {

    Renderer::Renderer() {
        m_verticiesLength = 0;
    }

    Renderer::~Renderer() {

    }

    void Renderer::init(GLFWwindow* window) {
        m_window = window;
        m_shader.init("../../shaders/default.vert", "../../shaders/default.frag");
        createVertexArrayObject();
        createVertexBufferObject();
        glEnable(GL_DEPTH_TEST);
    };
}

// Private
namespace ObjectRenderer {

    void Renderer::createVertexArrayObject() {
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
    }

    void Renderer::createVertexBufferObject() {

        float* vertices = m_loader.getVertices();
        m_verticiesLength = m_loader.getVerticesLength();
        unsigned int* indices = m_loader.getEdges();
        m_indicesLength = m_loader.getEdgesLength();

        glGenBuffers(1, &m_EBO);
        glGenBuffers(1, &m_VBO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, m_verticiesLength * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesLength * sizeof(unsigned int), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Renderer::renderCycle() {

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        m_shader.use();
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, m_indicesLength, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(m_window);
        glfwPollEvents();

    }
}

