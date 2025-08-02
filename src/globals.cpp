#pragma once
#include "globals.hpp"
#include <string>
#include <vector>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "entityManagementSystem.hpp"
#include <iostream>

namespace ObjectRenderer {

    void g_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }

    void g_initImGui(GLFWwindow* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO(); (void)io;

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 450");
    }

    void g_cleanupImGui() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void g_createImGuiFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void g_renderImGui() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    std::string g_buildImGuiUIContent(std::vector< std::string>* entities)
    {
        ImGui::Begin("Scene Content");

        static int selectedIndex = 0;

        if (!entities || entities->empty())
        {
            ImGui::Text("No entities available.");
            ImGui::End();
            return "";
        }

        if (selectedIndex >= entities->size())
            selectedIndex = 0;

        const std::string& selectedItem = (*entities)[selectedIndex];

        if (ImGui::BeginListBox("Entities"))
        {
            for (int i = 0; i < entities->size(); i++)
            {
                bool isSelected = (selectedIndex == i);
                if (ImGui::Selectable((*entities)[i].c_str(), isSelected)) {
                    selectedIndex = i;
                    std::cout << (*entities)[i].c_str() << std::endl;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }

            }
        }
        ImGui::EndListBox();

        ImGui::End();
        return (*entities)[selectedIndex];

    }

    void g_buildImGuiUIProperties(EntityTransformation* properties)
    {
        float transform[] = { properties->translation.x, properties->translation.y, properties->translation.z };
        float scale[] = { properties->scale.x, properties->scale.y, properties->scale.z };
        float rotationDirection[] = { properties->rotationDirection.x, properties->rotationDirection.y, properties->rotationDirection.z };

        ImGui::Begin("Properties");

        ImGui::DragFloat3("Transformation", transform, 0.1f, -20.0f, 20.0f);
        ImGui::DragFloat("Angle", &properties->radianAngle, 1.0f, 0.0f, 360.0f);
        ImGui::DragFloat3("Rotation Direction", rotationDirection, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat3("Scale", scale, 0.1f, 0.0f, 20.0f);

        ImGui::End();

        properties->translation.x = transform[0];
        properties->translation.y = transform[1];
        properties->translation.z = transform[2];

        properties->scale.x = scale[0];
        properties->scale.y = scale[1];
        properties->scale.z = scale[2];

        properties->rotationDirection.x = rotationDirection[0];
        properties->rotationDirection.y = rotationDirection[1];
        properties->rotationDirection.z = rotationDirection[2];
    }

    void g_showDockspace() {
        static bool isFullScreen = true;
        static ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        if (isFullScreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGui::Begin("DockSpace Demo", nullptr, windowFlags);
        ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiID dockSpaceID = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockSpaceID, ImVec2(0.0f, 0.0f), dockSpaceFlags);

        ImGui::End();
    }

}