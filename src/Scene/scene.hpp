#pragma once

#include "entt/entt.hpp"
#include "../Entity/entity.hpp"
#include "../Shader/shader.hpp"
#include "../Entity/meshHandler.hpp"

namespace ObjectRenderer {
    class Scene {
    public:
        Scene();

        void update();
        void render(Shader& shader, MeshHandler& meshHandler);
        void renderLighting(Shader& shader, MeshHandler& meshHandler);
        void removeEntity(entt::entity entity);
        entt::entity createEntity();

        template <typename... Components, typename... Args>
        entt::entity createEntityWithComponents(Args&&... args) {
            entt::entity entity = EntityComponentSystem::create(m_registry);
            EntityComponentSystem::emplaceOrReplaceComponentInEntity<Components...>(m_registry, entity, std::forward<Args>(args)...);
            return entity;
        }

        template <typename... Components, typename... Args>
        void emplaceOrReplaceComponentInEntity(entt::entity entity, Args&&... args) {
            EntityComponentSystem::emplaceOrReplaceComponentInEntity<Components...>(m_registry, entity, std::forward<Args>(args)...);
        }

        template <typename... Components>
        auto viewEntitysWithComponents() {
            return EntityComponentSystem::getEntities<Components...>(m_registry);
        }

        template <typename Component>
        void removeComponentFromEntity(entt::entity entity) {
            EntityComponentSystem::removeComponentFromEntity<Component>(m_registry, entity);
        }

        template <typename Component>
        Component* getComponentFromEntity(entt::entity entity) {
            return EntityComponentSystem::getComponentFromEntity<Component>(m_registry, entity);
        }

        template <typename Component>
        bool isComponentInEntity(entt::entity entity) {
            return EntityComponentSystem::isComponentInEntity<Component>(m_registry, entity);
        }

    private:
        void renderMeshObjects(Shader& shader, MeshHandler& meshHandler);
        void renderLightingObjects(Shader& shader, MeshHandler& meshHandler);

        entt::registry m_registry;
    };
}