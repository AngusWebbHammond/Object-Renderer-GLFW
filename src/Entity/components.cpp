#pragma once

#include "components.h"
#include <iostream>
#include "../Core/globals.h"

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
        lightingType = POINT_LIGHT;
        direction = glm::vec3(0.0f, 0.0f, -1.0f);

        ambientFactor = 0.2f;
        diffuseFactor = 1.0f;
        specularFactor = 1.0f;

        spotLightCutOffAngle = 12.5f;
        spotLightOuterCutOffAngle = 17.5f;

        constantAttenuationFactor = 1.0f;
        linearAttenuationFactor = 0.09f;
        quadraticAttenuationFactor = 0.032f;

        isSoftEdges = false;
    }

    LightingComponent::LightingComponent(float _intensity)
    {
        intensity = _intensity;
        colour = glm::vec3(1.0f);
        lightingType = POINT_LIGHT;
        direction = glm::vec3(0.0f, 0.0f, -1.0f);

        ambientFactor = 0.2f;
        diffuseFactor = 1.0f;
        specularFactor = 1.0f;

        spotLightCutOffAngle = 12.5f;
        spotLightOuterCutOffAngle = 17.5f;

        constantAttenuationFactor = 1.0f;
        linearAttenuationFactor = 0.09f;
        quadraticAttenuationFactor = 0.032f;

        isSoftEdges = false;
    }

    LightingComponent::LightingComponent(float _intensity, glm::vec3 _colour)
    {
        intensity = _intensity;
        colour = _colour;
        lightingType = POINT_LIGHT;
        direction = glm::vec3(0.0f, 0.0f, -1.0f);

        ambientFactor = 0.2f;
        diffuseFactor = 1.0f;
        specularFactor = 1.0f;

        spotLightCutOffAngle = 12.5f;
        spotLightOuterCutOffAngle = 17.5f;

        constantAttenuationFactor = 1.0f;
        linearAttenuationFactor = 0.09f;
        quadraticAttenuationFactor = 0.032f;

        isSoftEdges = false;
    }

    LightingComponent::LightingComponent(float _intensity, glm::vec3 _colour, LightType _lightingType)
    {
        intensity = _intensity;
        colour = _colour;
        lightingType = _lightingType;
        direction = glm::vec3(0.0f, 0.0f, -1.0f);

        ambientFactor = 0.2f;
        diffuseFactor = 1.0f;
        specularFactor = 1.0f;

        spotLightCutOffAngle = 12.5f;
        spotLightOuterCutOffAngle = 17.5f;

        constantAttenuationFactor = 1.0f;
        linearAttenuationFactor = 0.09f;
        quadraticAttenuationFactor = 0.032f;

        isSoftEdges = false;
    }

    LightingComponent::LightingComponent(float _intensity, glm::vec3 _colour, LightType _lightingType,
        glm::vec3 _direction, float _ambientFactor, float _diffuseFactor,
        float _specularFactor, float _spotLightCutOffAngle, float _spotLightOuterCutOffAngle,
        float _constantAttenuationFactor, float _linearAttenuationFactor, float _quadraticAttenuationFactor,
        bool _isSoftEdges)
    {
        intensity = _intensity;
        colour = _colour;
        lightingType = _lightingType;
        direction = _direction;

        ambientFactor = _ambientFactor;
        diffuseFactor = _diffuseFactor;
        specularFactor = _specularFactor;

        spotLightCutOffAngle = _spotLightCutOffAngle;
        spotLightOuterCutOffAngle = _spotLightOuterCutOffAngle;

        constantAttenuationFactor = _constantAttenuationFactor;
        linearAttenuationFactor = _linearAttenuationFactor;
        quadraticAttenuationFactor = _quadraticAttenuationFactor;

        isSoftEdges = _isSoftEdges;
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

    CameraComponent::CameraComponent(float _yaw, float _pitch, bool _isPrimaryCamera)
    {
        yaw = _yaw;
        pitch = _pitch;

        worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

        isPrimaryCamera = _isPrimaryCamera;

        glm::vec3 frontTemp;
        frontTemp.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        frontTemp.y = sin(glm::radians(pitch));
        frontTemp.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        front = glm::normalize(frontTemp);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }

    CameraComponent::~CameraComponent()
    {
    }
}
