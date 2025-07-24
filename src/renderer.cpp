#pragma once
#include "renderer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include "globals.hpp"
#include "mesh.hpp"

// Public
namespace ObjectRenderer {

    Renderer::Renderer() {
        m_trianglesNumber = 0;
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
        // Loading the objects and getting the verticies for the input to the gl buffer
        std::vector<float> vertices = m_loader.getVertices();
        std::vector<float> normals = m_loader.getNormals();
        std::vector<float> textures = m_loader.getTextures();

        std::vector<int> edges = m_loader.getEdges();
        std::vector<int> textureEdges = m_loader.getTextureEdges();
        std::vector<int> normalEdges = m_loader.getNormalEdges();

        float colour[] = { 1.0f, 0.0f, 1.0f };

        m_meshHandler.addObject(vertices, textures, edges, colour, normals, normalEdges, textureEdges, "Cube");

        std::vector<float> coords = m_meshHandler.getVerticies();
        m_trianglesNumber = static_cast<int>(coords.size());

        // OpenGL Code
        glGenBuffers(1, &m_VBO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(float), coords.data(), GL_STATIC_DRAW);

        // Position Data
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

        // Colour Data
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Surface Normal Data
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // Texture Data
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void*)(9 * sizeof(float)));
        glEnableVertexAttribArray(3);

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
        glDrawArrays(GL_TRIANGLES, 0, m_trianglesNumber);
        glBindVertexArray(0);

        glfwSwapBuffers(m_window);
        glfwPollEvents();

    }
}

