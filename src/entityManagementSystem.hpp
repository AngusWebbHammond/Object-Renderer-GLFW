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
        float radianAngle;
        glm::vec3 rotationDirection;

        EntityTransformation();
        EntityTransformation(glm::vec3 location);
        EntityTransformation(glm::vec3 location, float angle, glm::vec3 rotate);
    };

    class EntityManager
    {
    public:
        EntityManager();
        void addModel(std::string name);
        void addModel(std::string name, glm::vec3 location);
        void addModel(std::string name, glm::vec3 location, float angle, glm::vec3 rotationDirection);

        std::map<std::string, std::vector<EntityTransformation>> getModels();

    private:
        std::map<std::string, std::vector<EntityTransformation>> m_models;
    };
}
