#pragma once

#include "components.hpp"
#include <iostream>
#include "../Core/globals.hpp"

namespace EntityComponentSystem {

    TransformComponent::TransformComponent()
    {
        translation = glm::vec3(0.0f);
        rotationAngles = glm::vec3(0.0f);
        scale = glm::vec3(1.0f);
    }

    LightingComponent::LightingComponent()
    {
        intensity = 1.0f;
        colour = glm::vec3(1.0f);
    }

    LightingComponent::LightingComponent(float _intensity)
    {
        intensity = _intensity;
        colour = glm::vec3(1.0f);
    }

    LightingComponent::LightingComponent(float _intensity, glm::vec3 _colour)
    {
        intensity = _intensity;
        colour = _colour;
    }

    NameComponent::NameComponent()
    {
        name = "New Entity";
    }

    NameComponent::NameComponent(entt::entity entity)
    {
        name = "New Entity " + std::to_string(static_cast<int>(entity));
    }

    MeshComponent::MeshComponent()
    {
        meshName = "";
    }

    MeshComponent::MeshComponent(std::string _meshName)
    {
        meshName = _meshName;
    }

    CameraComponent::CameraComponent()
    {
        yaw = ObjectRenderer::g_yaw;
        pitch = ObjectRenderer::g_pitch;

        front = glm::vec3(0.0f, 0.0f, -1.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        right = glm::vec3(1.0f, 0.0f, 0.0f);
        worldUp = up;
    }

    CameraComponent::~CameraComponent()
    {
    }
}
