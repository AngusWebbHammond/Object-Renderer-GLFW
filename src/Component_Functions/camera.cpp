#pragma once

#include "camera.h"

#include <iostream>

namespace ObjectRenderer::Camera {

    namespace {
        void updateCamera(EntityComponentSystem::CameraComponent& cameraComponent)
        {
            glm::vec3 front;
            front.x = cos(glm::radians(cameraComponent.yaw)) * cos(glm::radians(cameraComponent.pitch));
            front.y = sin(glm::radians(cameraComponent.pitch));
            front.z = sin(glm::radians(cameraComponent.yaw)) * cos(glm::radians(cameraComponent.pitch));

            cameraComponent.front = glm::normalize(front);
            cameraComponent.right = glm::normalize(glm::cross(cameraComponent.front, cameraComponent.worldUp));
            cameraComponent.up = glm::normalize(glm::cross(cameraComponent.right, cameraComponent.front));
        }
    }

    void onKeyboardPress(EntityComponentSystem::TransformComponent& transformComponent, EntityComponentSystem::CameraComponent& cameraComponent, CameraMovement direction, float deltaTime)
    {
        float velocity = g_speed * deltaTime;

        if (direction == FORWARD) {
            transformComponent.translation += cameraComponent.front * velocity;
        }
        if (direction == BACKWARD) {
            transformComponent.translation -= cameraComponent.front * velocity;
        }
        if (direction == LEFT) {
            transformComponent.translation -= cameraComponent.right * velocity;
        }
        if (direction == RIGHT) {
            transformComponent.translation += cameraComponent.right * velocity;
        }
    }

    glm::mat4 getViewMatrix(EntityComponentSystem::TransformComponent& transformComponent, EntityComponentSystem::CameraComponent& cameraComponent)
    {
        return glm::lookAt(transformComponent.translation, transformComponent.translation + cameraComponent.front, cameraComponent.up);
    }

    void onMouseMoveUpdateCamera(EntityComponentSystem::CameraComponent& cameraComponent, float xOffset, float yOffset, bool constrainPitch)
    {
        xOffset *= ObjectRenderer::g_sensitivity;
        yOffset *= ObjectRenderer::g_sensitivity;

        cameraComponent.yaw += xOffset;
        cameraComponent.pitch += yOffset;

        // Prevent screen flip
        if (constrainPitch) {
            if (cameraComponent.pitch > 89.0f)
                cameraComponent.pitch = 89.0f;
            if (cameraComponent.pitch < -89.0f)
                cameraComponent.pitch = -89.0f;
        }

        updateCamera(cameraComponent);
    }

}

