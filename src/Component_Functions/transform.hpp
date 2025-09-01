#pragma once

#include "../Entity/components.hpp"

namespace ObjectRenderer::Transform {
    glm::mat4 getModelMatrix(EntityComponentSystem::TransformComponent& transformComponent);
}