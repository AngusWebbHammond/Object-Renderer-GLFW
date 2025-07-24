#pragma once
#include "mesh.hpp"
#include <map>
#include <string>

namespace ObjectRenderer {
    struct TriangleObjectPoints {
        int startIndex;
        int endIndex;

        TriangleObjectPoints();
        TriangleObjectPoints(int start, int end);
    };

    class MeshHandler {
    public:
        MeshHandler();
        ~MeshHandler();

        void addObject(std::vector<float> positions, std::vector<float> textures,
            std::vector<int> triangleVerticies, float* colour,
            std::vector<float> surfaceNormals, std::vector<int> normalVerticies,
            std::vector<int> textureVerticies, std::string meshName);

        std::vector<float> getVerticies();

    private:
        std::vector<std::shared_ptr<Vertex>> m_verticies;
        std::vector<std::shared_ptr<Texture>> m_textures;
        std::vector<std::shared_ptr<Triangle>> m_triangles;
        std::vector<std::shared_ptr<Mesh>> m_meshObjects;

        std::map<std::string, TriangleObjectPoints> m_triangleObjectPoints;

        std::vector<float> m_completeVerticies;

        void addVertices(std::vector<float> positions);
        void addTextures(std::vector<float> textures);
        void addTriangles(std::vector<int> triangleVerticies, float* colour,
            std::vector<float> surfaceNormals, std::vector<int> normalVerticies,
            std::vector<int> textureVerticies, std::string meshName);
        void addMesh(std::string meshName);
        void generateVerticies();
    };
}