#pragma once

#ifndef GLAD_GL_IMPLEMENTATION
#define GLAD_GL_IMPLEMENTATION
#endif

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

#include "../Scene/scene.hpp"

namespace UI {
    void initImGui(GLFWwindow* window);
    void cleanupImGui();
    void createImGuiFrame();
    void renderImGui();
    entt::entity buildImGuiUIContent(ObjectRenderer::Scene& scene);
    void buildImGuiUIProperties(ObjectRenderer::Scene& scene, entt::entity& entity);
    void showDockspace();
}