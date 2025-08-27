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
        TransformComponent(glm::vec3 _position);
    };

    struct MeshComponent {
        std::string meshName;
        glm::vec3 colour;

        MeshComponent();
        MeshComponent(std::string _meshName);
        MeshComponent(std::string _meshName, glm::vec3 _colour);
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
        NameComponent(std::string _name);
    };

    struct CameraComponent {
        float yaw;
        float pitch;

        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;

        bool isPrimaryCamera;

        CameraComponent();
        CameraComponent(bool _isPrimaryCamera);
        ~CameraComponent();
    };
}
