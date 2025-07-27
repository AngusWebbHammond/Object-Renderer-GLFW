#pragma once
#include "renderer.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include "globals.hpp"
#include "mesh.hpp"
#include "entityManagementSystem.hpp"

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
        m_objectLoader.loadObjectFromFile("../../objects/cube.obj");

        std::vector<float> vertices = m_objectLoader.getVertices();
        std::vector<float> normals = m_objectLoader.getNormals();
        std::vector<float> textures = m_objectLoader.getTextures();

        std::vector<int> edges = m_objectLoader.getEdges();
        std::vector<int> textureEdges = m_objectLoader.getTextureEdges();
        std::vector<int> normalEdges = m_objectLoader.getNormalEdges();

        float colour[] = { 1.0f, 0.0f, 1.0f };

        m_meshHandler.addObject(vertices, textures, edges, colour, normals, normalEdges, textureEdges, "Cube");

        std::vector<float> coords = m_meshHandler.getVerticies();
        m_trianglesNumber = static_cast<int>(coords.size());

        m_entityManager.addModel("Cube");
        m_entityManager.addModel("Cube", glm::vec3(0.0f, 4.0f, 2.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f));

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



        std::map<std::string, std::vector<EntityTransformation>> models = m_entityManager.getModels();

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        m_lightingPosition = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(0.2f), glm::vec3(1.0, 1.0, 0.0)) * glm::vec4(m_lightingPosition, 1.0f));

        projection = glm::perspective(glm::radians(45.0f), (float)g_width / (float)g_height, 0.1f, 100.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f));

        m_shader.setMat4("view", view);
        m_shader.setMat4("projection", projection);
        m_shader.setVec3("lightingPosition", m_lightingPosition);

        glBindVertexArray(m_VAO);

        for (auto i = models.begin(); i != models.end(); ++i) {

            std::vector<EntityTransformation> currentVector = models[i->first];
            TriangleObjectPoints meshPoints = m_meshHandler.getMeshIndexStartEnd(i->first);
            int startIndex = meshPoints.startIndex * 33;
            int endIndex = meshPoints.endIndex * 33;
            int length = endIndex - startIndex;

            for (int j = 0; j < currentVector.size(); j++) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, currentVector[j].translation);
                model = glm::rotate(model, glm::radians(currentVector[j].radianAngle), currentVector[j].rotationDirection);
                model = glm::scale(model, currentVector[j].scale);
                m_shader.setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, startIndex, endIndex - startIndex);

                std::cout << "Drawing Model " << j << std::endl;
                std::cout << "Location {" << currentVector[j].translation.x << ", " << currentVector[j].translation.y << ", " << currentVector[j].translation.z << "}" << std::endl;
                std::cout << "Rotation direction {" << currentVector[j].rotationDirection.x << ", " << currentVector[j].rotationDirection.y << ", " << currentVector[j].rotationDirection.z << "}" << std::endl;
            }
        }
        glfwSwapBuffers(m_window);
        glfwPollEvents();

    }
}

