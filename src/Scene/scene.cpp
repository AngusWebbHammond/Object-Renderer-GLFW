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
            shader.setVec3("lightingColour", lightingComponent.colour);
            shader.setFloat("lightingIntensity", lightingComponent.intensity);

            TriangleObjectPoints meshPoints = meshHandler.getMeshIndexStartEnd(meshComponent.meshName);
            int startIndex = meshPoints.startIndex;
            int endIndex = meshPoints.endIndex;
            int length = endIndex - startIndex;

            meshHandler.bindMeshVAO(meshComponent.meshName);

            shader.setMat4("model", Transform::getModelMatrix(transformComponent));

            glDrawArrays(GL_TRIANGLES, startIndex * 3, length * 3);

            glBindVertexArray(0);

        }
    }

    void Scene::renderOutline(Shader& shader, MeshHandler& meshHandler, entt::entity& entity)
    {
        auto meshComponent = getComponentFromEntity<EntityComponentSystem::MeshComponent>(entity);
        auto transformComponent = getComponentFromEntity<EntityComponentSystem::TransformComponent>(entity);

        TriangleObjectPoints meshPoints = meshHandler.getMeshIndexStartEnd(meshComponent->meshName);
        int startIndex = meshPoints.startIndex;
        int endIndex = meshPoints.endIndex;
        int length = endIndex - startIndex;

        meshHandler.bindMeshVAO(meshComponent->meshName);

        shader.setMat4("model", Transform::getModelMatrix(*transformComponent));

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glDrawArrays(GL_TRIANGLES, startIndex * 3, length * 3);

        glCullFace(GL_BACK);

        glBindVertexArray(0);

    }

    void Scene::saveSceneToFile()
    {
        YAML::Node root;

        auto entities = EntityComponentSystem::getEntities<EntityComponentSystem::NameComponent>(m_registry);

        for (auto [entity, nameComponent] : entities.each())
        {
            YAML::Node node;

            if (isComponentInEntity<EntityComponentSystem::TransformComponent>(entity)) {
                EntityComponentSystem::TransformComponent* transformComponent = getComponentFromEntity<EntityComponentSystem::TransformComponent>(entity);
                node["TransformComponent"]["translation"] = vec3ToYaml(transformComponent->translation);
                node["TransformComponent"]["rotationAngles"] = vec3ToYaml(transformComponent->rotationAngles);
                node["TransformComponent"]["scale"] = vec3ToYaml(transformComponent->scale);
            }

            if (isComponentInEntity<EntityComponentSystem::MeshComponent>(entity)) {
                EntityComponentSystem::MeshComponent* meshComponent = getComponentFromEntity<EntityComponentSystem::MeshComponent>(entity);
                node["MeshComponent"]["meshName"] = meshComponent->meshName;
                node["MeshComponent"]["colour"] = vec3ToYaml(meshComponent->colour);
            }

            if (isComponentInEntity<EntityComponentSystem::LightingComponent>(entity)) {
                EntityComponentSystem::LightingComponent* lightingComponent = getComponentFromEntity<EntityComponentSystem::LightingComponent>(entity);
                node["LightingComponent"]["intensity"] = lightingComponent->intensity;
                node["LightingComponent"]["colour"] = vec3ToYaml(lightingComponent->colour);
            }

            if (isComponentInEntity<EntityComponentSystem::LightingComponent>(entity)) {
                EntityComponentSystem::NameComponent* nameComponent = getComponentFromEntity<EntityComponentSystem::NameComponent>(entity);
                node["NameComponent"]["name"] = nameComponent->name;
            }

            if (isComponentInEntity<EntityComponentSystem::CameraComponent>(entity)) {
                EntityComponentSystem::CameraComponent* cameraComponent = getComponentFromEntity<EntityComponentSystem::CameraComponent>(entity);
                node["CameraComponent"]["yaw"] = cameraComponent->yaw;
                node["CameraComponent"]["pitch"] = cameraComponent->pitch;
                node["CameraComponent"]["front"] = vec3ToYaml(cameraComponent->front);
                node["CameraComponent"]["up"] = vec3ToYaml(cameraComponent->up);
                node["CameraComponent"]["right"] = vec3ToYaml(cameraComponent->right);
                node["CameraComponent"]["worldUp"] = vec3ToYaml(cameraComponent->worldUp);
            }

            root["entities"][nameComponent.name] = node;
        }

        std::string fileOut{ (std::filesystem::path(PROJECT_DIR) / "assets" / "scenes" / "scene.yaml").string() };
        std::ofstream fout(fileOut);
        fout << root;
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

            shader.setMat4("model", Transform::getModelMatrix(transformComponent));

            shader.setVec3("meshColour", meshComponent.colour);

            glDrawArrays(GL_TRIANGLES, startIndex * 3, length * 3);

            glBindVertexArray(0);
        }
    }

    void Scene::addLightingToShader(Shader& shader, MeshHandler& meshHandler)
    {
        auto entities = EntityComponentSystem::getEntities<EntityComponentSystem::LightingComponent, EntityComponentSystem::TransformComponent>(m_registry);

        for (auto [entity, lightingComponent, transformComponent] : entities.each()) {
            shader.setVec3("lightingPosition", transformComponent.translation);
            shader.setVec3("lightingColour", lightingComponent.colour);
            shader.setFloat("lightingIntensity", lightingComponent.intensity);
        }
    }

    YAML::Node Scene::vec3ToYaml(const glm::vec3& v) {
        YAML::Node node;
        node.push_back(v.x);
        node.push_back(v.y);
        node.push_back(v.z);
        return node;
    }
}
