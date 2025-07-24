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
        m_verticiesLength = 0;
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

        float* vertices = m_loader.getVertices();
        float* normals = m_loader.getNormals();
        float* textures = m_loader.getTextures();

        m_verticiesLength = m_loader.getVerticesLength();
        m_texturesLength = m_loader.getTexturesLength();
        m_normalsLength = m_loader.getNormalsLength();

        int* edges = m_loader.getEdges();
        int* textureEdges = m_loader.getTextureEdges();
        int* normalEdges = m_loader.getNormalEdges();

        m_edgesLength = m_loader.getEdgesLength();

        float colour[] = { 0.5f, 0.5f, 0.5f };

        m_meshHandler.addObject(vertices, m_verticiesLength, textures, m_texturesLength, edges, colour, normals, normalEdges, textureEdges, m_edgesLength, "Cube");

        float* coords = m_meshHandler.getVerticies();
        int coordsLength = m_meshHandler.getVerticiesLength();

        float coordNew[g_maxBufferSize];

        for (int i = 0; i < coordsLength; i++) {
            coordNew[i] = coords[i];
        }

        m_trianglesNumber += coordsLength / 11;

        // ---------------------------------------------------------------------------

        // m_meshHandler.addObject(vertices, textures, normals, indices, m_verticiesLength, m_indicesLength);

        glGenBuffers(1, &m_VBO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, coordsLength * sizeof(float), coordNew, GL_STATIC_DRAW);

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

