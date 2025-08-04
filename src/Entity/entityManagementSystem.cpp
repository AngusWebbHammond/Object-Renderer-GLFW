#pragma once

#include "entityManagementSystem.hpp"

namespace ObjectRenderer {
    EntityManager::EntityManager()
    {
    }


    void EntityManager::addModel(std::string name, glm::vec3 locationVector)
    {
        if (m_models.find(name) == m_models.end()) {
            std::vector<EntityTransformation> newVector(1, EntityTransformation(locationVector));
            m_models[name] = newVector;
        }
        else {
            m_models[name].push_back(EntityTransformation(locationVector));
        }
    }

    void EntityManager::addModel(std::string name, glm::vec3 locationVector, glm::vec3 rotationVector, glm::vec3 scaleVector)
    {
        if (m_models.find(name) == m_models.end()) {
            std::vector<EntityTransformation> newVector(1, EntityTransformation(locationVector, rotationVector, scaleVector));
            m_models[name] = newVector;
        }
        else {
            m_models[name].push_back(EntityTransformation(locationVector, rotationVector, scaleVector));
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
        rotationAngles = glm::vec3(0.0f);
        scale = glm::vec3(1.0f);
    }

    EntityTransformation::EntityTransformation(glm::vec3 locationVector)
    {
        translation = locationVector;
        rotationAngles = glm::vec3(0.0f);
        scale = glm::vec3(1.0f);
    }

    EntityTransformation::EntityTransformation(glm::vec3 locationVector, glm::vec3 rotationVector, glm::vec3 scaleVector)
    {
        translation = locationVector;
        rotationAngles = rotationVector;
        scale = scaleVector;
    }
}
