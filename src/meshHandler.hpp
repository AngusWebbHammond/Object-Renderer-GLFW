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

        void addObject(float* positions, int verticiesNumber, float* textures, int texturesNumber, int* triangleVerticies, float* colour, float* surfaceNormals, int* normalVerticies, int* textureVerticies, int triangleNumber, std::string meshName);

        float* getVerticies();
        int getVerticiesLength();

    private:
        std::shared_ptr<Vertex> m_verticies[g_maxBufferSize];
        std::shared_ptr<Texture> m_textures[g_maxBufferSize];
        std::shared_ptr<Triangle> m_triangles[g_maxBufferSize];
        std::shared_ptr<Mesh> m_meshObjects[g_maxBufferSize];

        int m_vertexLength;
        int m_texturesLength;
        int m_triangleLength;
        int m_meshObjectLength;

        std::map<std::string, TriangleObjectPoints> m_triangleObjectPoints;

        float m_completeVerticies[g_maxBufferSize];
        int m_completeVerticiesLength;

        void addVertices(float* positions, int verticiesNumber);
        void addTextures(float* textures, int texturesNumber);
        void addTriangles(int* triangleVerticies, float* colour, float* surfaceNormals, int* normalVerticies, int* textureVerticies, int triangleNumber, std::string meshName);
        void addMesh(std::string meshName);
        void generateVerticies();
    };
}