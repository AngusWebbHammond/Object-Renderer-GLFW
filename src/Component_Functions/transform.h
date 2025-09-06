#pragma once

#include "../Entity/components.h"

namespace ObjectRenderer::Transform {
    glm::mat4 getModelMatrix(EntityComponentSystem::TransformComponent& transformComponent);
}