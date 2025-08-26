#pragma once

#include "camera.hpp"

#include <iostream>

namespace ObjectRenderer {

    Camera::Camera()
    {
        m_position = glm::vec3(0.0f, 0.0f, 3.0f);
        m_up = glm::vec3(0.0f, 1.0f, 0.0f);
        m_front = glm::vec3(0.0f, 0.0f, -1.0f);
        m_right = glm::vec3(1.0f, 0.0f, 0.0f);
        m_worldUp = m_up;

        m_yaw = g_yaw;
        m_pitch = g_pitch;

        m_movementSpeed = g_speed;
        m_mouseSensitivity = g_sensitivity;

        updateCamera();
    }

    Camera::~Camera()
    {
    }

    glm::mat4 Camera::getViewMatrix()
    {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    void Camera::onKeyboardPress(CameraMovement direction, float delatTime)
    {
        float velocity = m_movementSpeed * delatTime;

        if (direction == FORWARD) {
            m_position += m_front * velocity;
        }
        if (direction == BACKWARD) {
            m_position -= m_front * velocity;
        }
        if (direction == LEFT) {
            m_position -= m_right * velocity;
        }
        if (direction == RIGHT) {
            m_position += m_right * velocity;
        }
    }

    void Camera::onMouseMove(float xOffset, float yOffset, bool constrainPitch)
    {
        xOffset *= m_mouseSensitivity;
        yOffset *= m_mouseSensitivity;

        m_yaw += xOffset;
        m_pitch += yOffset;

        // Prevent screen flip
        if (constrainPitch) {
            if (m_pitch > 89.0f)
                m_pitch = 89.0f;
            if (m_pitch < -89.0f)
                m_pitch = -89.0f;
        }

        updateCamera();
    }

    void Camera::updateCamera()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        front.y = sin(glm::radians(m_pitch));
        front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

        m_front = glm::normalize(front);
        m_right = glm::normalize(glm::cross(m_front, m_worldUp));
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }
}

namespace ObjectRenderer::Cameras {

    glm::mat4 getViewMatrix(const EntityComponentSystem::TransformComponent& transformComponent, const EntityComponentSystem::CameraComponent& cameraComponent)
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

        glm::vec3 front;
        front.x = cos(glm::radians(cameraComponent.yaw)) * cos(glm::radians(cameraComponent.pitch));
        front.y = sin(glm::radians(cameraComponent.pitch));
        front.z = sin(glm::radians(cameraComponent.yaw)) * cos(glm::radians(cameraComponent.pitch));

        cameraComponent.front = glm::normalize(front);
        cameraComponent.right = glm::normalize(glm::cross(cameraComponent.front, cameraComponent.worldUp));
        cameraComponent.up = glm::normalize(glm::cross(cameraComponent.right, cameraComponent.front));
    }
}

