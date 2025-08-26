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
        // auto entities = EntityComponentSystem::getEntities<>(m_registry);
        // for (auto entity : entities) {
        //     if (EntityComponentSystem::isComponentInEntity<EntityComponentSystem::TransformComponent>(m_registry, entity)) {

        //     }
        // }
    }

    void Scene::render(Shader& shader, MeshHandler& meshHandler)
    {
        renderMeshObjects(shader, meshHandler);
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
}
