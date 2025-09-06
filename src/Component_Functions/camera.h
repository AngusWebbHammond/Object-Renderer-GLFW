#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Core/globals.h"
#include "../Entity/components.h"

namespace ObjectRenderer::Camera {

    enum CameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    void onKeyboardPress(EntityComponentSystem::TransformComponent& transformComponent, EntityComponentSystem::CameraComponent& cameraComponent, CameraMovement direction, float deltaTime);
    glm::mat4 getViewMatrix(EntityComponentSystem::TransformComponent& transformComponent, EntityComponentSystem::CameraComponent& cameraComponent);
    void onMouseMoveUpdateCamera(EntityComponentSystem::CameraComponent& cameraComponent, float xOffset, float yOffset, bool constrainPitch = true);
}