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

        void addObject(float* positions, int verticiesNumber,
            float* textures, int texturesNumber,
            int* triangleVerticies, float* colour,
            float* surfaceNormals, int* normalVerticies,
            int* textureVerticies, int triangleNumber,
            std::string meshName);

        std::vector<float> getVerticies();

    private:
        std::vector<std::shared_ptr<Vertex>> m_verticies;
        std::vector<std::shared_ptr<Texture>> m_textures;
        std::vector<std::shared_ptr<Triangle>> m_triangles;
        std::vector<std::shared_ptr<Mesh>> m_meshObjects;

        std::map<std::string, TriangleObjectPoints> m_triangleObjectPoints;

        std::vector<float> m_completeVerticies;

        void addVertices(float* positions, int verticiesNumber);
        void addTextures(float* textures, int texturesNumber);
        void addTriangles(int* triangleVerticies, float* colour,
            float* surfaceNormals, int* normalVerticies,
            int* textureVerticies, int triangleNumber,
            std::string meshName);
        void addMesh(std::string meshName);
        void generateVerticies();
    };
}