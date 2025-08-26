#pragma once

#include <glm/glm.hpp>
#include <string>
#include <entt/entity/fwd.hpp>

namespace EntityComponentSystem {

    struct TransformComponent {
        glm::vec3 translation;
        glm::vec3 rotationAngles;
        glm::vec3 scale;

        TransformComponent();
    };

    struct MeshComponent {
        std::string meshName;

        MeshComponent();
        MeshComponent(std::string _meshName);
    };

    struct LightingComponent {
        glm::float32 intensity;
        glm::vec3 colour;

        LightingComponent();
        LightingComponent(float _intensity);
        LightingComponent(float _intensity, glm::vec3 _colour);
    };

    struct NameComponent {
        std::string name;

        NameComponent();
        NameComponent(entt::entity entity);
    };

    struct CameraComponent {
        float yaw;
        float pitch;

        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;

        CameraComponent();
        ~CameraComponent();
    };
}
