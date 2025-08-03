#pragma once
#include <map>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ObjectRenderer
{
    struct EntityTransformation {
        glm::vec3 translation;
        glm::vec3 rotationAngles;
        glm::vec3 scale;

        EntityTransformation();
        EntityTransformation(glm::vec3 location);
        EntityTransformation(glm::vec3 location, glm::vec3 rotationAngles, glm::vec3 scale);
    };

    class EntityManager
    {
    public:
        EntityManager();
        void addModel(std::string name);
        void addModel(std::string name, glm::vec3 location);
        void EntityManager::addModel(std::string name, glm::vec3 locationVector, glm::vec3 rotationVector, glm::vec3 scaleVector);

        std::map<std::string, std::vector<EntityTransformation>>* getModels();

    private:
        std::map<std::string, std::vector<EntityTransformation>> m_models;
    };
}
