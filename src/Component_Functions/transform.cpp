#pragma once

#include "transform.h"
#include <glm/ext/matrix_transform.hpp>

namespace ObjectRenderer::Transform {
    glm::mat4 getModelMatrix(EntityComponentSystem::TransformComponent& transformComponent)
    {
        glm::mat4 model{ 1.0f };
        model = glm::translate(model, transformComponent.translation);
        model = glm::rotate(model, glm::radians(transformComponent.rotationAngles.x), { 1.0f, 0.0f, 0.0f });
        model = glm::rotate(model, glm::radians(transformComponent.rotationAngles.y), { 0.0f, 1.0f, 0.0f });
        model = glm::rotate(model, glm::radians(transformComponent.rotationAngles.z), { 0.0f, 0.0f, 1.0f });
        model = glm::scale(model, transformComponent.scale);

        return model;
    }
}