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
        ImGui::Begin("Properties");

        if (ImGui::CollapsingHeader("Transformations")) {
            ImGui::Text("Translation");
            ImGui::DragFloat("X", &properties->translation.x, 0.01f, -20.0f, 20.0f);
            ImGui::DragFloat("Y", &properties->translation.y, 0.01f, -20.0f, 20.0f);
            ImGui::DragFloat("Z", &properties->translation.z, 0.01f, -20.0f, 20.0f);

            ImGui::Text("Rotation");
            ImGui::DragFloat("Angle X", &properties->rotationAngles.x, 0.1f, -180.0f, 180.0f);
            ImGui::DragFloat("Angle Y", &properties->rotationAngles.y, 0.1f, -180.0f, 180.0f);
            ImGui::DragFloat("Angle Z", &properties->rotationAngles.z, 0.1f, -180.0f, 180.0f);

            ImGui::Text("Scale");
            ImGui::DragFloat("Scale X", &properties->scale.x, 0.01f, 0.0f, 20.0f);
            ImGui::DragFloat("Scale Y", &properties->scale.y, 0.01f, 0.0f, 20.0f);
            ImGui::DragFloat("Scale Z", &properties->scale.z, 0.01f, 0.0f, 20.0f);
        }

        ImGui::End();
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