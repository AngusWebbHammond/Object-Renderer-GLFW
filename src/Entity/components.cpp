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

    TransformComponent::TransformComponent(glm::vec3 _position)
    {
        translation = _position;
        rotationAngles = glm::vec3(0.0f);
        scale = glm::vec3(1.0f);
    }

    TransformComponent::TransformComponent(glm::vec3 _position, glm::vec3 _rotationAngles, glm::vec3 _scale)
    {
        translation = _position;
        rotationAngles = _rotationAngles;
        scale = _scale;
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

    NameComponent::NameComponent(std::string _name)
    {
        name = _name;
    }

    MeshComponent::MeshComponent()
    {

        meshName = "Cube";
        colour = glm::vec3{ 0.5f, 0.5f, 0.5f };
    }

    MeshComponent::MeshComponent(std::string _meshName)
    {
        meshName = _meshName;
        colour = glm::vec3{ 0.5f, 0.5f, 0.5f };
    }

    MeshComponent::MeshComponent(std::string _meshName, glm::vec3 _colour)
    {
        meshName = _meshName;
        colour = _colour;
    }

    CameraComponent::CameraComponent()
    {
        yaw = ObjectRenderer::g_yaw;
        pitch = ObjectRenderer::g_pitch;

        front = glm::vec3(0.0f, 0.0f, -1.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        right = glm::vec3(1.0f, 0.0f, 0.0f);
        worldUp = up;

        isPrimaryCamera = false;
    }

    CameraComponent::CameraComponent(bool _isPrimaryCamera)
    {
        yaw = ObjectRenderer::g_yaw;
        pitch = ObjectRenderer::g_pitch;

        front = glm::vec3(0.0f, 0.0f, -1.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        right = glm::vec3(1.0f, 0.0f, 0.0f);
        worldUp = up;

        isPrimaryCamera = _isPrimaryCamera;
    }

    CameraComponent::CameraComponent(float _yaw, float _pitch, glm::vec3 _front, glm::vec3 _up, glm::vec3 _right, glm::vec3 _worldUp, bool _isPrimaryCamera)
    {
        yaw = _yaw;
        pitch = _pitch;

        front = _front;
        up = _up;
        right = _right;
        worldUp = _worldUp;

        isPrimaryCamera = _isPrimaryCamera;
    }

    CameraComponent::~CameraComponent()
    {
    }
}
