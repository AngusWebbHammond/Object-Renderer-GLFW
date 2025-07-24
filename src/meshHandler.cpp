#pragma once
#include "meshHandler.hpp"
#include "objectLoader.hpp"
#include <list>

namespace ObjectRenderer {
    TriangleObjectPoints::TriangleObjectPoints()
    {
        startIndex = 0;
        endIndex = 0;
    }

    TriangleObjectPoints::TriangleObjectPoints(int start, int end)
    {
        startIndex = start;
        endIndex = end;
    }

    MeshHandler::MeshHandler()
    {
        m_vertexLength = 0;
        m_triangleLength = 0;
        m_meshObjectLength = 0;
        m_completeVerticiesLength = 0;
    }

    MeshHandler::~MeshHandler()
    {
    }

    void MeshHandler::addObject(float* positions, int verticiesNumber, float* textures, int texturesNumber, int* triangleVerticies, float* colour, float* surfaceNormals, int* normalVerticies, int* textureVerticies, int triangleNumber, std::string meshName)
    {
        addVertices(positions, verticiesNumber);
        addTextures(textures, texturesNumber);
        addTriangles(triangleVerticies, colour, surfaceNormals, normalVerticies, textureVerticies, triangleNumber, meshName);
        addMesh(meshName);

        generateVerticies();
    }

    float* MeshHandler::getVerticies() {
        return m_completeVerticies;
    }

    int MeshHandler::getVerticiesLength() {
        return m_completeVerticiesLength;
    }

    void MeshHandler::addVertices(float* positions, int verticiesNumber)
    {
        float* currentPosition;
        for (int i = 0; i < verticiesNumber / 3; i++) {
            currentPosition = &positions[i * 3];
            m_verticies[i] = std::make_shared<Vertex>(currentPosition);
        }

        m_vertexLength += verticiesNumber;
    }

    void MeshHandler::addTextures(float* textures, int texturesNumber)
    {
        float* currentTexture;
        for (int i = 0; i < texturesNumber / 2; i++) {
            currentTexture = &textures[i * 2];
            m_textures[i] = std::make_shared<Texture>(currentTexture);
        }

        m_texturesLength += texturesNumber;
    }

    void MeshHandler::addTriangles(int* triangleVerticies, float* colour, float* surfaceNormals, int* normalVerticies, int* textureVerticies, int trianglesNumber, std::string meshName)
    {
        std::shared_ptr<Vertex> currentTriangleVerticies[3];
        std::shared_ptr<Texture> currentTextureVerticies[3];
        float* currentSurfaceNormal;

        int count = 0;

        for (int i = 0; i < trianglesNumber / 3; i++) {
            for (int j = 0; j < 3; j++) {
                currentTriangleVerticies[j] = m_verticies[triangleVerticies[3 * i + j]];
                currentTextureVerticies[j] = m_textures[textureVerticies[3 * i + j]];
            }

            currentSurfaceNormal = &surfaceNormals[3 * normalVerticies[3 * i]];

            m_triangles[i] = std::make_shared<Triangle>(currentTriangleVerticies, currentTextureVerticies, currentSurfaceNormal, colour);
            count++;
        }

        m_triangleObjectPoints[meshName] = TriangleObjectPoints(m_triangleLength, m_triangleLength + count);
        m_triangleLength += count;
    }

    void MeshHandler::addMesh(std::string meshName)
    {
        int start = m_triangleObjectPoints[meshName].startIndex;
        int end = m_triangleObjectPoints[meshName].endIndex;

        int length = end - start;

        std::shared_ptr<Triangle> triangles[g_maxBufferSize];

        for (int i = start; i < end; i++) {
            triangles[i - start] = m_triangles[i];
        }

        m_meshObjects[m_meshObjectLength] = std::make_shared<Mesh>(triangles, length);

        m_meshObjectLength++;
    }

    void MeshHandler::generateVerticies()
    {
        std::shared_ptr<Mesh> currentMesh;
        int currentMeshVerticiesLength;
        float* currentMeshVerticies;

        for (int i = 0; i < m_meshObjectLength; i++) {
            currentMesh = m_meshObjects[i];
            currentMeshVerticiesLength = currentMesh->getVerticiesLength();
            currentMeshVerticies = currentMesh->getVerticies();

            for (int j = 0; j < currentMeshVerticiesLength; j++) {
                m_completeVerticies[m_completeVerticiesLength + j] = currentMeshVerticies[j];
            }

            m_completeVerticiesLength += currentMeshVerticiesLength;
        }
    };
}
