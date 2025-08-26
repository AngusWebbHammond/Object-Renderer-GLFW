#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../Core/globals.hpp"
#include "../Entity/components.hpp"

namespace ObjectRenderer {

    enum CameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    class Camera {
    public:
        Camera();
        ~Camera();

        glm::mat4 getViewMatrix();
        void onKeyboardPress(CameraMovement direction, float delatTime);
        void onMouseMove(float xOffset, float yOffset, bool constrainPitch = true);

    private:
        void updateCamera();

    private:
        float m_yaw;
        float m_pitch;

        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_up;
        glm::vec3 m_right;
        glm::vec3 m_worldUp;

        float m_movementSpeed;
        float m_mouseSensitivity;
    };
}

namespace ObjectRenderer::Cameras {
    glm::mat4 getViewMatrix(const EntityComponentSystem::TransformComponent& transformComponent, const EntityComponentSystem::CameraComponent& cameraComponent);
    void onMouseMoveUpdateCamera(EntityComponentSystem::CameraComponent& cameraComponent, float xOffset, float yOffset, bool constrainPitch = true);
}