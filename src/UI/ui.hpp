#pragma once

#ifndef GLAD_GL_IMPLEMENTATION
#define GLAD_GL_IMPLEMENTATION
#endif

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

#include "../Entity/entityManagementSystem.hpp"

namespace UI {
    void initImGui(GLFWwindow* window);
    void cleanupImGui();
    void createImGuiFrame();
    void renderImGui();
    std::string buildImGuiUIContent(std::vector<std::string>* entities);
    void buildImGuiUIProperties(ObjectRenderer::EntityTransformation* properties);
    void showDockspace();
}