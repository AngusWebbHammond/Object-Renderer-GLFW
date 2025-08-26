#pragma once

#include "ui.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "../Scene/scene.hpp"

namespace UI {
    void initImGui(GLFWwindow* window)
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

    void cleanupImGui() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void createImGuiFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void renderImGui() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    entt::entity buildImGuiUIContent(ObjectRenderer::Scene& scene)
    {
        ImGui::Begin("Scene Content");

        auto entities = scene.viewEntitysWithComponents<EntityComponentSystem::NameComponent>();

        static int selectedIndex = 0;
        static entt::entity selectedEntity;

        if (entities.empty())
        {
            ImGui::Text("No entities available.");
            ImGui::End();
            return selectedEntity;
        }

        if (selectedIndex >= entities->size())
            selectedIndex = 0;

        if (ImGui::BeginListBox("Entities"))
        {
            int i = 0;
            for (auto [entity, name] : entities.each())
            {
                bool isSelected = (selectedIndex == i);

                if (ImGui::Selectable(name.name.c_str(), isSelected)) {
                    selectedIndex = i;
                    selectedEntity = entity;
                }

                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }

                i++;
            }

            ImGui::EndListBox();
        }
        ImGui::End();
        return selectedEntity;
    }

    void buildImGuiUIProperties(ObjectRenderer::Scene& scene, entt::entity& entity)
    {
        ImGui::Begin("Properties");

        if (scene.isComponentInEntity<EntityComponentSystem::TransformComponent>(entity))
        {
            EntityComponentSystem::TransformComponent* transformComponent = scene.getComponentFromEntity<EntityComponentSystem::TransformComponent>(entity);
            if (ImGui::CollapsingHeader("Transformations")) {
                ImGui::Text("Translation");
                ImGui::DragFloat("X", &transformComponent->translation.x, 0.01f, -20.0f, 20.0f);
                ImGui::DragFloat("Y", &transformComponent->translation.y, 0.01f, -20.0f, 20.0f);
                ImGui::DragFloat("Z", &transformComponent->translation.z, 0.01f, -20.0f, 20.0f);

                ImGui::Text("Rotation");
                ImGui::DragFloat("Angle X", &transformComponent->rotationAngles.x, 0.1f, -180.0f, 180.0f);
                ImGui::DragFloat("Angle Y", &transformComponent->rotationAngles.y, 0.1f, -180.0f, 180.0f);
                ImGui::DragFloat("Angle Z", &transformComponent->rotationAngles.z, 0.1f, -180.0f, 180.0f);

                ImGui::Text("Scale");
                ImGui::DragFloat("Scale X", &transformComponent->scale.x, 0.01f, 0.0f, 20.0f);
                ImGui::DragFloat("Scale Y", &transformComponent->scale.y, 0.01f, 0.0f, 20.0f);
                ImGui::DragFloat("Scale Z", &transformComponent->scale.z, 0.01f, 0.0f, 20.0f);
            }
        }

        ImGui::End();
    }

    // void buildImGuiUIProperties(entt:entity & entity)
    // {
    //     // ImGui::Begin("Properties");

    //     // if (ImGui::CollapsingHeader("Transformations")) {
    //     //     ImGui::Text("Translation");
    //     //     ImGui::DragFloat("X", &properties->translation.x, 0.01f, -20.0f, 20.0f);
    //     //     ImGui::DragFloat("Y", &properties->translation.y, 0.01f, -20.0f, 20.0f);
    //     //     ImGui::DragFloat("Z", &properties->translation.z, 0.01f, -20.0f, 20.0f);

    //     //     ImGui::Text("Rotation");
    //     //     ImGui::DragFloat("Angle X", &properties->rotationAngles.x, 0.1f, -180.0f, 180.0f);
    //     //     ImGui::DragFloat("Angle Y", &properties->rotationAngles.y, 0.1f, -180.0f, 180.0f);
    //     //     ImGui::DragFloat("Angle Z", &properties->rotationAngles.z, 0.1f, -180.0f, 180.0f);

    //     //     ImGui::Text("Scale");
    //     //     ImGui::DragFloat("Scale X", &properties->scale.x, 0.01f, 0.0f, 20.0f);
    //     //     ImGui::DragFloat("Scale Y", &properties->scale.y, 0.01f, 0.0f, 20.0f);
    //     //     ImGui::DragFloat("Scale Z", &properties->scale.z, 0.01f, 0.0f, 20.0f);
    //     // }

    //     // ImGui::End();
    // }

    void showDockspace() {
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