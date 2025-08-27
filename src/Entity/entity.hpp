#pragma once

#include "components.hpp"
#include "entt/entt.hpp"

namespace EntityComponentSystem {

    entt::entity create(entt::registry& registry);
    void removeEntity(entt::registry& registry, entt::entity entity);

    template <typename T, typename... Args>
    void emplaceOrReplaceComponentInEntity(entt::registry& registry, entt::entity entity, Args&&... args)
    {
        registry.emplace_or_replace<T>(entity, std::forward<Args>(args)...);
    }

    template <typename T>
    void removeComponentFromEntity(entt::registry& registry, entt::entity entity)
    {
        registry.erase<T>(entity);
    }

    template <typename... Component>
    auto getEntities(entt::registry& registry) {
        return registry.view<Component...>();
    }

    template <typename Component>
    Component* getComponentFromEntity(entt::registry& registry, entt::entity entity) {
        return &registry.get<Component>(entity);
    }

    template <typename Component>
    bool isComponentInEntity(entt::registry& registry, entt::entity entity) {
        return registry.all_of<Component>(entity);
    }
}