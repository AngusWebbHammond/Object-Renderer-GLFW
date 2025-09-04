#pragma once

#include <glm/glm.hpp>
#include <string>
#include <entt/entity/fwd.hpp>
#include <tuple>

namespace EntityComponentSystem {

    struct TransformComponent {
        glm::vec3 translation;
        glm::vec3 rotationAngles;
        glm::vec3 scale;

        TransformComponent();
        TransformComponent(glm::vec3 _position);
        TransformComponent(glm::vec3 _position, glm::vec3 _rotationAngles, glm::vec3 _scale);
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
        CameraComponent(float _yaw, float _pitch, glm::vec3 _front, glm::vec3 _up, glm::vec3 _right, glm::vec3 _worldUp, bool _isPrimaryCamera);
        CameraComponent(float _yaw, float _pitch, bool _isPrimaryCamera);
        ~CameraComponent();
    };

    using Components = std::tuple<TransformComponent, MeshComponent, LightingComponent, NameComponent, CameraComponent>;

    template <typename... Ts, typename Func>
    void forEachType(std::tuple<Ts...>, Func&& func) {
        (func.template operator() < Ts > (), ...);
    }

    template <typename T>
    constexpr const char* componentName();

#define REGISTER_COMPONENT_NAME(type) \
    template <> constexpr const char* componentName<type>() { return #type; }

    REGISTER_COMPONENT_NAME(TransformComponent);
    REGISTER_COMPONENT_NAME(MeshComponent);
    REGISTER_COMPONENT_NAME(LightingComponent);
    REGISTER_COMPONENT_NAME(NameComponent);
    REGISTER_COMPONENT_NAME(CameraComponent);
}
