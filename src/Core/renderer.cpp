#pragma once

#include "renderer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <map>
#include <vector>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "globals.hpp"
#include "../Entity/mesh.hpp"
#include "../UI/ui.hpp"

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
        initFrameBuffer();
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

        auto entity = m_scene.createEntity();
        m_scene.addComponentToEnity<EntityComponentSystem::MeshComponent>(entity, "Cube");

        m_scene.createEntity();

        m_scene.createEntityWithComponents<EntityComponentSystem::MeshComponent>("Torus");

        m_scene.createEntityWithComponents<EntityComponentSystem::MeshComponent>("Suzanne");

        // OpenGL Code

        glGenBuffers(1, &m_VBO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(float), coords.data(), GL_STATIC_DRAW);

        for (auto name : m_meshHandler.getVAONames()) {
            m_meshHandler.bindMeshVAO(name);

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

            glBindVertexArray(0);
        }


        glBindBuffer(GL_ARRAY_BUFFER, 0);
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
    }

    void Renderer::processMouse()
    {
        if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            m_canHandleMouse = true;
        }

        if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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

    void Renderer::initFrameBuffer()
    {
        glGenTextures(1, &m_colorTexture);
        glBindTexture(GL_TEXTURE_2D, m_colorTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_width, g_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Create depth buffer
        glGenRenderbuffers(1, &m_depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, g_width, g_height);

        // Create framebuffer
        glGenFramebuffers(1, &m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

        // Check
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Renderer::resizeFramebuffer(int width, int height)
    {
        if (width <= 0 || height <= 0)
            return;

        glBindTexture(GL_TEXTURE_2D, m_colorTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "Framebuffer incomplete after resize!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Renderer::renderCycle() {
        glfwPollEvents();

        UI::createImGuiFrame();

        UI::showDockspace();

        ImVec2 viewportPanelSize = ImVec2(0, 0);

        entt::entity selectedModel = UI::buildImGuiUIContent(m_scene);

        if (ImGui::Begin("Viewport")) {

            viewportPanelSize = ImGui::GetContentRegionAvail();

            if (m_colorTexture && viewportPanelSize.x > 0 && viewportPanelSize.y > 0) {
                ImGui::Image((void*)(intptr_t)m_colorTexture, viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));
            }

            if (ImGui::IsItemHovered()) {
                processMouse();
            }

            ImGui::End();
        }

        int fbWidth = static_cast<int>(viewportPanelSize.x);
        int fbHeight = static_cast<int>(viewportPanelSize.y);

        if (fbWidth == 0 || fbHeight == 0) {
            UI::renderImGui();
            glfwSwapBuffers(m_window);
            return;
        }


        if (fbWidth != m_lastFbWidth || fbHeight != m_lastFbHeight) {
            m_lastFbWidth = fbWidth;
            m_lastFbHeight = fbHeight;
            resizeFramebuffer(fbWidth, fbHeight);
        }

        UI::buildImGuiUIProperties(m_scene, selectedModel);

        // glClear(GL_COLOR_BUFFER_BIT);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Framebuffer incomplete!" << std::endl;
            UI::renderImGui();
            glfwSwapBuffers(m_window);
            return;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        glViewport(0, 0, fbWidth, fbHeight);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        m_shader.use();

        processInput(m_window, 0.05f);

        glm::mat4 view = m_camera.getViewMatrix();
        glm::mat4 projection = glm::mat4(1.0f);

        m_lightingPosition = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(0.2f), glm::vec3(1.0, 1.0, 0.0)) * glm::vec4(m_lightingPosition, 1.0f));

        if (fbHeight > 0 && fbWidth > 0) {
            projection = glm::perspective(glm::radians(45.0f), (float)fbWidth / (float)fbHeight, 0.1f, 100.0f);
        }

        m_shader.setMat4("view", view);
        m_shader.setMat4("projection", projection);
        m_shader.setVec3("lightingPosition", m_lightingPosition);

        m_scene.render(m_shader, m_meshHandler);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        UI::renderImGui();
        glfwSwapBuffers(m_window);

    }
}

