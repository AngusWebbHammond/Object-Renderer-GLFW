#pragma once

#include "ui.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "../Scene/scene.hpp"
#include <filesystem>
#include <tuple>

#include <windows.h>
#include <commdlg.h>
#include <string>

namespace UI {
    namespace {
        std::string OpenFileDialog(const char* filter = "All Files\0*.*\0Text Files\0*.txt\0") {
            char fileName[MAX_PATH] = "";
            OPENFILENAME ofn = {};
            ofn.lStructSize = sizeof(ofn);
            ofn.lpstrFile = fileName;
            ofn.nMaxFile = MAX_PATH;
            ofn.lpstrFilter = filter;
            ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

            if (GetOpenFileName(&ofn)) {
                return std::string(fileName);
            }
            return "";
        }

        std::string SaveFileDialog(const char* filter = "All Files\0*.*\0Text Files\0*.txt\0") {
            char fileName[MAX_PATH] = "";

            OPENFILENAME ofn = {};
            ofn.lStructSize = sizeof(ofn);
            ofn.lpstrFile = fileName;
            ofn.nMaxFile = MAX_PATH;
            ofn.lpstrFilter = filter;
            ofn.lpstrDefExt = "txt";
            ofn.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;

            if (GetSaveFileName(&ofn)) {
                return std::string(fileName);
            }

            return "";
        }
    }

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
        static entt::entity selectedEntity = entt::null;

        if (ImGui::Button("Add Entity"))
        {
            scene.createEntity();
        }

        if (entities.empty())
        {
            ImGui::Text("No entities available.");
            selectedEntity = entt::null;
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
        }

        ImGui::End();

        return selectedEntity;
    }

    void buildImGuiUIProperties(ObjectRenderer::Scene& scene, entt::entity& entity)
    {
        ImGui::Begin("Properties");

        if (entity == entt::null) {
            ImGui::Text("No Entity Selected.");
            ImGui::End();
            return;
        }

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

                const char* lightTypes[] = { "Point", "Directional", "Spot" };
                int currentLight = static_cast<int>(lightingComponent->lightingType);

                if (ImGui::BeginCombo("Light Type", lightTypes[lightingComponent->lightingType])) {
                    for (int i = 0; i < IM_ARRAYSIZE(lightTypes); i++) {
                        bool isSelected = (currentLight == i);
                        if (ImGui::Selectable(lightTypes[i], isSelected)) {
                            currentLight = i;
                            lightingComponent->lightingType = static_cast<EntityComponentSystem::LightingComponent::LightType>(i);
                        }
                        if (isSelected) {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }

                if (lightingComponent->lightingType == EntityComponentSystem::LightingComponent::DIRECTIONAL_LIGHT || lightingComponent->lightingType == EntityComponentSystem::LightingComponent::SPOT_LIGHT) {
                    ImGui::Text("Direction");
                    ImGui::DragFloat("Direction X", &lightingComponent->direction.x, 0.01f, -1.0f, 1.0f);
                    ImGui::DragFloat("Direction Y", &lightingComponent->direction.y, 0.01f, -1.0f, 1.0f);
                    ImGui::DragFloat("Direction Z", &lightingComponent->direction.z, 0.01f, -1.0f, 1.0f);
                }
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


    void showDockspace(ObjectRenderer::Scene& scene) {
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

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Scene")) {
                if (ImGui::MenuItem("New")) {
                    scene.clearScene();
                    entt::entity entity = scene.createEntity();
                    scene.emplaceOrReplaceComponentInEntity<EntityComponentSystem::MeshComponent>(entity, "Cube");
                    scene.emplaceOrReplaceComponentInEntity<EntityComponentSystem::NameComponent>(entity, "Cube");

                    entity = scene.createEntity();
                    scene.emplaceOrReplaceComponentInEntity<EntityComponentSystem::TransformComponent>(entity, glm::vec3(-4.5f, 2.5f, 5.0f));
                    scene.emplaceOrReplaceComponentInEntity<EntityComponentSystem::CameraComponent>(entity, -48.6f, -17.4f, true);
                    scene.emplaceOrReplaceComponentInEntity<EntityComponentSystem::NameComponent>(entity, "Camera");

                    entity = scene.createEntity();
                    scene.emplaceOrReplaceComponentInEntity<EntityComponentSystem::TransformComponent>(entity, glm::vec3(0.0f, 4.0f, 4.0f));
                    scene.emplaceOrReplaceComponentInEntity<EntityComponentSystem::LightingComponent>(entity);
                    scene.emplaceOrReplaceComponentInEntity<EntityComponentSystem::NameComponent>(entity, "Light");
                }
                if (ImGui::MenuItem("Save")) {
                    std::string path = SaveFileDialog("YAML Files\0*.yaml\0All Files\0*.*\0");
                    if (!path.empty()) {
                        scene.saveSceneToFile(path);
                    }
                }
                if (ImGui::MenuItem("Load")) {
                    std::string path = OpenFileDialog("YAML Files\0*.yaml\0All Files\0*.*\0");
                    if (!path.empty()) {
                        scene.loadSceneFromFile(path.c_str());
                    }
                }
                ImGui::EndMenu();
            }
        }

        ImGui::EndMenuBar();

        ImGui::End();

        if (!std::filesystem::exists("imgui.ini"))
        {
            ImGui::LoadIniSettingsFromDisk((std::filesystem::path(PROJECT_DIR) / "src" / "UI" / "DefaultPanelLayout.ini").string().c_str());
        }
    }
}
