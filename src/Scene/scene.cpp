#pragma once

#include "scene.hpp"
#include "../Component_Functions/camera.hpp"
#include <entt/entt.hpp>
#include <iostream>
#include <filesystem>
#include "yaml-cpp/yaml.h"
#include "../Component_Functions/transform.hpp"

namespace ObjectRenderer {

    Scene::Scene()
    {
    }

    void Scene::update()
    {
    }

    void Scene::render(Shader& shader, MeshHandler& meshHandler)
    {
        addLightingToShader(shader, meshHandler);
        renderMeshObjects(shader, meshHandler);
    }

    void Scene::renderLighting(Shader& shader, MeshHandler& meshHandler)
    {
        auto entities = EntityComponentSystem::getEntities<EntityComponentSystem::LightingComponent, EntityComponentSystem::TransformComponent, EntityComponentSystem::MeshComponent>(m_registry);

        for (auto [entity, lightingComponent, transformComponent, meshComponent] : entities.each()) {

            if (lightingComponent.lightingType == EntityComponentSystem::LightingComponent::DIRECTIONAL_LIGHT) continue;

            shader.setVec3("light.colour", lightingComponent.colour);
            shader.setFloat("light.intensity", lightingComponent.intensity);

            TriangleObjectPoints meshPoints = meshHandler.getMeshIndexStartEnd(meshComponent.meshName);
            int startIndex = meshPoints.startIndex;
            int endIndex = meshPoints.endIndex;
            int length = endIndex - startIndex;

            meshHandler.bindMeshVAO(meshComponent.meshName);

            shader.setMat4("modelViewProjection.model", Transform::getModelMatrix(transformComponent));

            glDrawArrays(GL_TRIANGLES, startIndex * 3, length * 3);

            glBindVertexArray(0);

        }
    }

    void Scene::renderOutline(Shader& shader, MeshHandler& meshHandler, entt::entity& entity)
    {
        if (entity == entt::null) return;

        if (!(isComponentInEntity<EntityComponentSystem::MeshComponent>(entity))) return;

        if (isComponentInEntity<EntityComponentSystem::LightingComponent>(entity)) {
            if (getComponentFromEntity<EntityComponentSystem::LightingComponent>(entity)->lightingType
                == EntityComponentSystem::LightingComponent::DIRECTIONAL_LIGHT)
                return;
        }

        auto meshComponent = getComponentFromEntity<EntityComponentSystem::MeshComponent>(entity);
        auto transformComponent = getComponentFromEntity<EntityComponentSystem::TransformComponent>(entity);

        TriangleObjectPoints meshPoints = meshHandler.getMeshIndexStartEnd(meshComponent->meshName);
        int startIndex = meshPoints.startIndex;
        int endIndex = meshPoints.endIndex;
        int length = endIndex - startIndex;

        meshHandler.bindMeshVAO(meshComponent->meshName);

        shader.setMat4("modelViewProjection.model", Transform::getModelMatrix(*transformComponent));

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glDrawArrays(GL_TRIANGLES, startIndex * 3, length * 3);

        glCullFace(GL_BACK);

        glBindVertexArray(0);

    }

    void Scene::saveSceneToFile(const std::string& fileOut)
    {
        YAML::Node root;

        auto entities = EntityComponentSystem::getEntities<EntityComponentSystem::NameComponent>(m_registry);

        for (auto [entity, nameComponent] : entities.each())
        {
            YAML::Node node;

            if (isComponentInEntity<EntityComponentSystem::TransformComponent>(entity)) {
                EntityComponentSystem::TransformComponent* transformComponent = getComponentFromEntity<EntityComponentSystem::TransformComponent>(entity);
                node["TransformComponent"]["translation"] = transformComponent->translation;
                node["TransformComponent"]["rotationAngles"] = transformComponent->rotationAngles;
                node["TransformComponent"]["scale"] = transformComponent->scale;
            }

            if (isComponentInEntity<EntityComponentSystem::MeshComponent>(entity)) {
                EntityComponentSystem::MeshComponent* meshComponent = getComponentFromEntity<EntityComponentSystem::MeshComponent>(entity);
                node["MeshComponent"]["meshName"] = meshComponent->meshName;
                node["MeshComponent"]["colour"] = meshComponent->colour;
            }

            if (isComponentInEntity<EntityComponentSystem::LightingComponent>(entity)) {
                EntityComponentSystem::LightingComponent* lightingComponent = getComponentFromEntity<EntityComponentSystem::LightingComponent>(entity);
                node["LightingComponent"]["intensity"] = lightingComponent->intensity;
                node["LightingComponent"]["colour"] = lightingComponent->colour;
            }

            if (isComponentInEntity<EntityComponentSystem::LightingComponent>(entity)) {
                EntityComponentSystem::NameComponent* nameComponent = getComponentFromEntity<EntityComponentSystem::NameComponent>(entity);
                node["NameComponent"]["name"] = nameComponent->name;
            }

            if (isComponentInEntity<EntityComponentSystem::CameraComponent>(entity)) {
                EntityComponentSystem::CameraComponent* cameraComponent = getComponentFromEntity<EntityComponentSystem::CameraComponent>(entity);
                node["CameraComponent"]["yaw"] = cameraComponent->yaw;
                node["CameraComponent"]["pitch"] = cameraComponent->pitch;
                node["CameraComponent"]["front"] = cameraComponent->front;
                node["CameraComponent"]["up"] = cameraComponent->up;
                node["CameraComponent"]["right"] = cameraComponent->right;
                node["CameraComponent"]["worldUp"] = cameraComponent->worldUp;
                node["CameraComponent"]["isPrimaryCamera"] = cameraComponent->isPrimaryCamera;
            }

            root["entities"][nameComponent.name] = node;
        }

        std::ofstream fout(fileOut);
        fout << root;
    }

    void Scene::loadSceneFromFile(const char* fileName)
    {
        YAML::Node sceneFileNode = YAML::LoadFile(fileName);

        if (!sceneFileNode) return;

        m_registry.clear();

        for (auto it : sceneFileNode["entities"]) {
            std::string entityName = it.first.as<std::string>();
            YAML::Node entityNode = it.second;

            entt::entity entity = EntityComponentSystem::create(m_registry);
            EntityComponentSystem::emplaceOrReplaceComponentInEntity<EntityComponentSystem::NameComponent>(m_registry, entity, entityName);

            if (entityNode["TransformComponent"]) {
                glm::vec3 pos = entityNode["TransformComponent"]["translation"].as<glm::vec3>();
                glm::vec3 rot = entityNode["TransformComponent"]["rotationAngles"].as<glm::vec3>();
                glm::vec3 scale = entityNode["TransformComponent"]["scale"].as<glm::vec3>();

                EntityComponentSystem::emplaceOrReplaceComponentInEntity<EntityComponentSystem::TransformComponent>(m_registry, entity, pos, rot, scale);
            }

            if (entityNode["MeshComponent"]) {
                std::string meshName = entityNode["MeshComponent"]["meshName"].as<std::string>();
                glm::vec3 colour = entityNode["MeshComponent"]["colour"].as<glm::vec3>();

                EntityComponentSystem::emplaceOrReplaceComponentInEntity<EntityComponentSystem::MeshComponent>(m_registry, entity, meshName, colour);
            }

            if (entityNode["LightingComponent"]) {
                float intensity = entityNode["LightingComponent"]["intensity"].as<float>();
                glm::vec3 colour = entityNode["LightingComponent"]["colour"].as<glm::vec3>();

                EntityComponentSystem::emplaceOrReplaceComponentInEntity<EntityComponentSystem::LightingComponent>(m_registry, entity, intensity, colour);
            }

            if (entityNode["CameraComponent"]) {
                float yaw = entityNode["CameraComponent"]["yaw"].as<float>();
                float pitch = entityNode["CameraComponent"]["pitch"].as<float>();

                glm::vec3 front = entityNode["CameraComponent"]["front"].as<glm::vec3>();
                glm::vec3 up = entityNode["CameraComponent"]["up"].as<glm::vec3>();
                glm::vec3 right = entityNode["CameraComponent"]["right"].as<glm::vec3>();
                glm::vec3 worldUp = entityNode["CameraComponent"]["worldUp"].as<glm::vec3>();

                bool isPrimaryCamera = entityNode["CameraComponent"]["isPrimaryCamera"].as<bool>();

                EntityComponentSystem::emplaceOrReplaceComponentInEntity<EntityComponentSystem::CameraComponent>(m_registry, entity, yaw, pitch, front, up, right, worldUp, isPrimaryCamera);
            }
        }
    }

    void Scene::clearScene()
    {
        m_registry.clear();
    }

    void Scene::removeEntity(entt::entity entity)
    {
        EntityComponentSystem::removeEntity(m_registry, entity);
    }

    entt::entity Scene::createEntity()
    {
        return EntityComponentSystem::create(m_registry);
    }

    void Scene::renderMeshObjects(Shader& shader, MeshHandler& meshHandler)
    {
        auto entities = EntityComponentSystem::getEntities<EntityComponentSystem::TransformComponent, EntityComponentSystem::MeshComponent>(m_registry);

        for (auto [entity, transformComponent, meshComponent] : entities.each()) {
            if (isComponentInEntity<EntityComponentSystem::LightingComponent>(entity)) continue;
            TriangleObjectPoints meshPoints = meshHandler.getMeshIndexStartEnd(meshComponent.meshName);
            int startIndex = meshPoints.startIndex;
            int endIndex = meshPoints.endIndex;
            int length = endIndex - startIndex;

            meshHandler.bindMeshVAO(meshComponent.meshName);

            shader.setMat4("modelViewProjection.model", Transform::getModelMatrix(transformComponent));

            shader.setVec3("meshColour", meshComponent.colour);

            glDrawArrays(GL_TRIANGLES, startIndex * 3, length * 3);

            glBindVertexArray(0);
        }
    }

    void Scene::addLightingToShader(Shader& shader, MeshHandler& meshHandler)
    {
        auto entities = EntityComponentSystem::getEntities<EntityComponentSystem::LightingComponent, EntityComponentSystem::TransformComponent>(m_registry);

        // Point Lights
        int numPointLights = 0;
        int numDirectionalLights = 0;
        int numSpotLights = 0;

        for (auto [entity, lightingComponent, transformComponent] : entities.each()) {
            if (lightingComponent.lightingType == EntityComponentSystem::LightingComponent::POINT_LIGHT) {
                std::string prefix = "pointLights[" + std::to_string(numPointLights) + "]";
                shader.setVec3(prefix + ".position", transformComponent.translation);
                shader.setVec3(prefix + ".colour", lightingComponent.colour);
                shader.setFloat(prefix + ".intensity", lightingComponent.intensity);

                shader.setFloat(prefix + ".ambient", 0.2f);
                shader.setFloat(prefix + ".specular", 1.0f);
                shader.setFloat(prefix + ".diffuse", 1.0f);

                shader.setFloat(prefix + ".constantAttenuationFactor", 1.0f);
                shader.setFloat(prefix + ".linearAttenuationFactor", 0.09f);
                shader.setFloat(prefix + ".quadraticAttenuationFactor", 0.032f);
                numPointLights++;
            }

            else if (lightingComponent.lightingType == EntityComponentSystem::LightingComponent::DIRECTIONAL_LIGHT) {
                std::string prefix = "directionalLights[" + std::to_string(numDirectionalLights) + "]";
                shader.setVec3(prefix + ".direction", lightingComponent.direction);
                shader.setVec3(prefix + ".colour", lightingComponent.colour);
                shader.setFloat(prefix + ".intensity", lightingComponent.intensity);

                shader.setFloat(prefix + ".ambient", 0.2f);
                shader.setFloat(prefix + ".specular", 1.0f);
                shader.setFloat(prefix + ".diffuse", 1.0f);
                numDirectionalLights++;
            }

            if (lightingComponent.lightingType == EntityComponentSystem::LightingComponent::SPOT_LIGHT) {
                std::string prefix = "spotLights[" + std::to_string(numSpotLights) + "]";
                shader.setVec3(prefix + ".position", transformComponent.translation);
                shader.setVec3(prefix + ".colour", lightingComponent.colour);
                shader.setVec3(prefix + ".direction", lightingComponent.direction);

                shader.setFloat(prefix + ".intensity", lightingComponent.intensity);

                shader.setFloat(prefix + ".cutOff", glm::cos(glm::radians(12.5f)));
                shader.setFloat(prefix + ".outerCutOff", glm::cos(glm::radians(12.5f))); // Used for soft edges

                shader.setFloat(prefix + ".ambient", 0.2f);
                shader.setFloat(prefix + ".specular", 1.0f);
                shader.setFloat(prefix + ".diffuse", 1.0f);

                shader.setFloat(prefix + ".constantAttenuationFactor", 1.0f);
                shader.setFloat(prefix + ".linearAttenuationFactor", 0.09f);
                shader.setFloat(prefix + ".quadraticAttenuationFactor", 0.032f);
                numSpotLights++;
            }
        }

        shader.setInt("numPointLights", numPointLights);
        shader.setInt("numDirectionalLights", numDirectionalLights);
        shader.setInt("numSpotLights", numSpotLights);
    }
}

namespace YAML {
    template<>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs) {
            if (!node.IsSequence() || node.size() != 3) {
                return false;
            }
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };
}
