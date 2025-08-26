#pragma once

#include "entity.hpp"

namespace EntityComponentSystem {

    entt::entity create(entt::registry& registry) {
        auto entity = registry.create();
        registry.emplace<TransformComponent>(entity);
        registry.emplace<NameComponent>(entity, entity);
        return entity;
    }

    void removeEntity(entt::registry& registry, entt::entity entity)
    {
        if (registry.valid(entity)) {
            registry.destroy(entity);
        }
    }
}

