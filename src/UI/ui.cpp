#pragma once

#include "ui.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "../Scene/scene.hpp"
#include <filesystem>
#include <tuple>

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

        static int selectedIndex = entities->size() - 1;
        static entt::entity selectedEntity;

        if (entities.empty())
        {
            ImGui::Text("No entities available.");
            ImGui::End();
            return selectedEntity;
        }

        if (selectedIndex >= entities->size())
            selectedEntity = entt::entity();

        if (ImGui::BeginListBox("Entities"))
        {
            for (auto [entity, name] : entities.each())
            {
                bool isSelected = (selectedIndex == static_cast<int>(entity));

                if (ImGui::Selectable(name.name.c_str(), isSelected)) {
                    selectedIndex = static_cast<int>(entity);
                }

                if (isSelected) {
                    selectedEntity = entity;
                    ImGui::SetItemDefaultFocus();
                }
            }

            ImGui::EndListBox();

            if (ImGui::Button("Add Entity"))
            {
                scene.createEntity();
            }
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

                if (!scene.isComponentInEntity<EntityComponentSystem::CameraComponent>(entity)) {

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
        }

        if (scene.isComponentInEntity<EntityComponentSystem::MeshComponent>(entity))
        {
            EntityComponentSystem::MeshComponent* meshComponent = scene.getComponentFromEntity<EntityComponentSystem::MeshComponent>(entity);

            if (ImGui::CollapsingHeader("Mesh")) {

                ImGui::Text("Mesh");
                ImGui::Text(meshComponent->meshName.c_str());
                ImGui::ColorPicker3("Colour", &meshComponent->colour[0]);
            }
        }

        if (scene.isComponentInEntity<EntityComponentSystem::NameComponent>(entity))
        {
            EntityComponentSystem::NameComponent* nameComponent = scene.getComponentFromEntity<EntityComponentSystem::NameComponent>(entity);
            if (ImGui::CollapsingHeader("Name")) {

                ImGui::Text("Name");
                ImGui::Text(nameComponent->name.c_str());
            }
        }

        if (scene.isComponentInEntity<EntityComponentSystem::CameraComponent>(entity))
        {
            EntityComponentSystem::CameraComponent* cameraComponent = scene.getComponentFromEntity<EntityComponentSystem::CameraComponent>(entity);
            if (ImGui::CollapsingHeader("Camera")) {

                ImGui::Checkbox("Primary", &cameraComponent->isPrimaryCamera);
            }
        }

        if (scene.isComponentInEntity<EntityComponentSystem::LightingComponent>(entity))
        {
            EntityComponentSystem::LightingComponent* lightingComponent = scene.getComponentFromEntity<EntityComponentSystem::LightingComponent>(entity);
            if (ImGui::CollapsingHeader("Lighting")) {

                ImGui::ColorPicker3("Primary", &lightingComponent->colour[0]);
                ImGui::DragFloat("Intensity", &lightingComponent->intensity, 0.01f, 0.0f, 1.0f);
            }
        }

        static bool addComponentListOpen = false;

        if (ImGui::Button("Add Component")) {
            if (addComponentListOpen) addComponentListOpen = false;
            else addComponentListOpen = true;
        }

        if (addComponentListOpen) {
            if (ImGui::BeginListBox("Components")) {

                EntityComponentSystem::forEachType(EntityComponentSystem::Components{}, [&scene, entity]<typename T>() {
                    if (!scene.isComponentInEntity<T>(entity)) {
                        if (ImGui::Selectable(EntityComponentSystem::componentName<T>())) {
                            scene.emplaceOrReplaceComponentInEntity<T>(entity);
                            addComponentListOpen = false;
                        }
                    }
                });

                ImGui::EndListBox();
            }
        }
        ImGui::End();
    }


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

        ImGui::Begin("DockSpace", nullptr, windowFlags);
        ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiID dockSpaceID = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockSpaceID, ImVec2(0.0f, 0.0f), dockSpaceFlags);

        ImGui::End();

        if (!std::filesystem::exists("imgui.ini"))
        {
            ImGui::LoadIniSettingsFromDisk("../../src/UI/DefaultPanelLayout.ini");
        }
    }
}