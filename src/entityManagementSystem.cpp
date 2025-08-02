#pragma once
#include "entityManagementSystem.hpp"

namespace ObjectRenderer {
    EntityManager::EntityManager()
    {
    }


    void EntityManager::addModel(std::string name, glm::vec3 location)
    {
        if (m_models.find(name) == m_models.end()) {
            std::vector<EntityTransformation> newVector(1, EntityTransformation(location));
            m_models[name] = newVector;
        }
        else {
            m_models[name].push_back(EntityTransformation(location));
        }
    }

    void EntityManager::addModel(std::string name, glm::vec3 location, float angle, glm::vec3 rotationDirection, glm::vec3 scale)
    {
        if (m_models.find(name) == m_models.end()) {
            std::vector<EntityTransformation> newVector(1, EntityTransformation(location, angle, rotationDirection, scale));
            m_models[name] = newVector;
        }
        else {
            m_models[name].push_back(EntityTransformation(location, angle, rotationDirection, scale));
        }
    }

    void EntityManager::addModel(std::string name)
    {
        if (m_models.find(name) == m_models.end()) {
            std::vector<EntityTransformation> newVector(1, EntityTransformation());
            m_models[name] = newVector;
        }
        else {
            m_models[name].push_back(EntityTransformation());
        }
    }

    std::map<std::string, std::vector<EntityTransformation>>* EntityManager::getModels()
    {
        return &m_models;
    }

    EntityTransformation::EntityTransformation()
    {
        translation = glm::vec3(0.0f);
        radianAngle = 0;
        rotationDirection = glm::vec3(1.0f);
        scale = glm::vec3(1.0f);
    }

    EntityTransformation::EntityTransformation(glm::vec3 location)
    {
        translation = location;
        radianAngle = 0;
        rotationDirection = glm::vec3(1.0f);
        scale = glm::vec3(1.0f);
    }

    EntityTransformation::EntityTransformation(glm::vec3 location, float angle, glm::vec3 rotate, glm::vec3 scaleVec3)
    {
        translation = location;
        radianAngle = angle;
        rotationDirection = rotate;
        scale = scaleVec3;
    }
}
