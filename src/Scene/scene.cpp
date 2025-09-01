#pragma once

#include "scene.hpp"
#include "../Camera/camera.hpp"
#include <entt/entt.hpp>
#include <iostream>

namespace ObjectRenderer {

    Scene::Scene()
    {
    }

    void Scene::update()
    {
    }

    void Scene::render(Shader& shader, MeshHandler& meshHandler)
    {
        renderLightingObjects(shader, meshHandler);
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

            glm::mat4 model{ 1.0f };
            model = glm::translate(model, transformComponent.translation);
            model = glm::rotate(model, glm::radians(transformComponent.rotationAngles.x), { 1.0f, 0.0f, 0.0f });
            model = glm::rotate(model, glm::radians(transformComponent.rotationAngles.y), { 0.0f, 1.0f, 0.0f });
            model = glm::rotate(model, glm::radians(transformComponent.rotationAngles.z), { 0.0f, 0.0f, 1.0f });
            model = glm::scale(model, transformComponent.scale);
            shader.setMat4("model", model);

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

        glm::mat4 model{ 1.0f };
        model = glm::translate(model, transformComponent->translation);
        model = glm::rotate(model, glm::radians(transformComponent->rotationAngles.x), { 1.0f, 0.0f, 0.0f });
        model = glm::rotate(model, glm::radians(transformComponent->rotationAngles.y), { 0.0f, 1.0f, 0.0f });
        model = glm::rotate(model, glm::radians(transformComponent->rotationAngles.z), { 0.0f, 0.0f, 1.0f });
        model = glm::scale(model, transformComponent->scale);
        shader.setMat4("model", model);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glDrawArrays(GL_TRIANGLES, startIndex * 3, length * 3);

        glCullFace(GL_BACK);

        glBindVertexArray(0);

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

            glm::mat4 model{ 1.0f };
            model = glm::translate(model, transformComponent.translation);
            model = glm::rotate(model, glm::radians(transformComponent.rotationAngles.x), { 1.0f, 0.0f, 0.0f });
            model = glm::rotate(model, glm::radians(transformComponent.rotationAngles.y), { 0.0f, 1.0f, 0.0f });
            model = glm::rotate(model, glm::radians(transformComponent.rotationAngles.z), { 0.0f, 0.0f, 1.0f });
            model = glm::scale(model, transformComponent.scale);
            shader.setMat4("model", model);

            shader.setVec3("meshColour", meshComponent.colour);

            glDrawArrays(GL_TRIANGLES, startIndex * 3, length * 3);

            glBindVertexArray(0);
        }
    }

    void Scene::renderLightingObjects(Shader& shader, MeshHandler& meshHandler)
    {
        auto entities = EntityComponentSystem::getEntities<EntityComponentSystem::LightingComponent, EntityComponentSystem::TransformComponent>(m_registry);

        for (auto [entity, lightingComponent, transformComponent] : entities.each()) {
            shader.setVec3("lightingPosition", transformComponent.translation);
            shader.setVec3("lightingColour", lightingComponent.colour);
            shader.setFloat("lightingIntensity", lightingComponent.intensity);
        }
    }
}
