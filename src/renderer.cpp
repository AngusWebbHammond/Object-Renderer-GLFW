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
    }

    Renderer::~Renderer() {
    }

    void Renderer::init(GLFWwindow* window) {
        m_window = window;
        m_shader.init("../../shaders/default.vert", "../../shaders/default.frag");
        createVertexBufferObject();
        glEnable(GL_DEPTH_TEST);
    };
}

// Private
namespace ObjectRenderer {

    void Renderer::createVertexBufferObject() {
        // Loading the objects and getting the verticies for the input to the gl buffer
        m_meshHandler.addObjectFromFile("../../objects/torus.obj");
        m_meshHandler.addObjectFromFile("../../objects/cube.obj");
        m_meshHandler.addObjectFromFile("../../objects/monkey.obj");

        std::vector<float> coords = m_meshHandler.getVerticies();

        m_entityManager.addModel("Cube");
        m_entityManager.addModel("Cube", glm::vec3(0.0f, 4.0f, 2.0f), 45.0f, glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f));
        m_entityManager.addModel("Torus", glm::vec3(0.0f, -8.0f, 3.0f), 30.0f, glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(2.0f, 1.0f, 1.5f));
        m_entityManager.addModel("Torus", glm::vec3(0.0f, -2.0f, -4.0f), -60.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f));
        m_entityManager.addModel("Suzanne", glm::vec3(0.0f, 0.0f, -3.0f), 50.0f, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        m_entityManager.addModel("Suzanne", glm::vec3(0.0f, 5.0f, 0.0f), -75.0f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

        // OpenGL Code

        glGenBuffers(1, &m_VBO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(float), coords.data(), GL_STATIC_DRAW);

        for (auto name : m_meshHandler.getVAONames()) {
            m_meshHandler.bindMeshVAO(name);

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

            glBindVertexArray(0);
        }

        // Position Data

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Renderer::drawMeshObjects()
    {
        std::map<std::string, std::vector<EntityTransformation>> models = m_entityManager.getModels();


        for (auto i = models.begin(); i != models.end(); ++i) {

            std::vector<EntityTransformation> currentVector = i->second;
            TriangleObjectPoints meshPoints = m_meshHandler.getMeshIndexStartEnd(i->first);
            int startIndex = meshPoints.startIndex;
            int endIndex = meshPoints.endIndex;
            int length = endIndex - startIndex;

            m_meshHandler.bindMeshVAO(i->first);

            for (int j = 0; j < currentVector.size(); j++) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, currentVector[j].translation);
                model = glm::rotate(model, glm::radians(currentVector[j].radianAngle), currentVector[j].rotationDirection);
                model = glm::scale(model, currentVector[j].scale);
                m_shader.setMat4("model", model);

                glDrawArrays(GL_TRIANGLES, startIndex * 3, length * 3);
            }
        }
    }

    void Renderer::processInput(GLFWwindow* window, float deltaTime)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            m_camera.onKeyboardPress(FORWARD, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            m_camera.onKeyboardPress(BACKWARD, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            m_camera.onKeyboardPress(LEFT, deltaTime);

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            m_camera.onKeyboardPress(RIGHT, deltaTime);

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            m_canHandleMouse = true;
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            m_canHandleMouse = false;
            m_firstMouse = true;
        }

        if (m_canHandleMouse)
            handleMouse();
    }

    void Renderer::handleMouse()
    {
        double xPos, yPos;
        glfwGetCursorPos(m_window, &xPos, &yPos);

        if (m_firstMouse) {
            m_lastX = xPos;
            m_lastY = yPos;
            m_firstMouse = false;
        }

        float xoffset = xPos - m_lastX;
        float yoffset = m_lastY - yPos;

        m_camera.onMouseMove(xPos - m_lastX, m_lastY - yPos);

        m_lastX = xPos;
        m_lastY = yPos;
    }

    void Renderer::renderCycle() {

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        m_shader.use();

        processInput(m_window, 0.05f);

        glm::mat4 view = m_camera.getViewMatrix();
        glm::mat4 projection = glm::mat4(1.0f);

        m_lightingPosition = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(0.2f), glm::vec3(1.0, 1.0, 0.0)) * glm::vec4(m_lightingPosition, 1.0f));

        projection = glm::perspective(glm::radians(45.0f), (float)g_width / (float)g_height, 0.1f, 100.0f);

        m_shader.setMat4("view", view);
        m_shader.setMat4("projection", projection);
        m_shader.setVec3("lightingPosition", m_lightingPosition);

        drawMeshObjects();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}

