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
    }

    MeshHandler::~MeshHandler()
    {
    }

    void MeshHandler::addObject(std::vector<float> positions, std::vector<float> textures,
        std::vector<int> triangleVerticies, float* colour,
        std::vector<float> surfaceNormals, std::vector<int> normalVerticies,
        std::vector<int> textureVerticies, std::string meshName)
    {
        addVertices(positions);
        addTextures(textures);
        addTriangles(triangleVerticies, colour, surfaceNormals,
            normalVerticies, textureVerticies, meshName);
        addMesh(meshName);

        generateVerticies();
    }

    void MeshHandler::addObjectFromFile(char* filePath)
    {
        m_objectLoader.loadObjectFromFile(filePath);

        std::vector<float> vertices = m_objectLoader.getVertices();
        std::vector<float> normals = m_objectLoader.getNormals();
        std::vector<float> textures = m_objectLoader.getTextures();

        std::vector<int> edges = m_objectLoader.getEdges();
        std::vector<int> textureEdges = m_objectLoader.getTextureEdges();
        std::vector<int> normalEdges = m_objectLoader.getNormalEdges();

        float colour[] = { 1.0f, 0.0f, 1.0f };

        addObject(vertices, textures, edges, colour, normals, normalEdges, textureEdges, "Cube");
    }

    std::vector<float> MeshHandler::getVerticies()
    {
        return m_completeVerticies;
    }

    TriangleObjectPoints MeshHandler::getMeshIndexStartEnd(std::string name)
    {
        return m_triangleObjectPoints[name];
    }

    void MeshHandler::addVertices(std::vector<float> positions)
    {
        float* currentPosition;
        for (int i = 0; i < static_cast<int>(positions.size()) / 3; i++) {
            currentPosition = &positions[i * 3];
            m_verticies.push_back(std::make_shared<Vertex>(currentPosition));
        }
    }

    void MeshHandler::addTextures(std::vector<float> textures)
    {
        std::vector<float> currentTexture;
        for (int i = 0; i < static_cast<int>(textures.size()) / 2; i++) {
            currentTexture.clear();
            currentTexture.push_back(textures[i * 2]);
            currentTexture.push_back(textures[i * 2 + 1]);
            m_textures.push_back(std::make_shared<Texture>(currentTexture));
        }

    }

    void MeshHandler::addTriangles(std::vector<int> triangleVerticies, float* colour,
        std::vector<float> surfaceNormals, std::vector<int> normalVerticies,
        std::vector<int> textureVerticies, std::string meshName)
    {
        std::shared_ptr<Vertex> currentTriangleVerticies[3];
        std::shared_ptr<Texture> currentTextureVerticies[3];
        float* currentSurfaceNormal;

        int count = 0;

        for (int i = 0; i < static_cast<int>(triangleVerticies.size()) / 3; i++) {
            for (int j = 0; j < 3; j++) {
                currentTriangleVerticies[j] = m_verticies[triangleVerticies[3 * i + j]];
                if (textureVerticies.size() < 2) {
                    currentTextureVerticies[j] = std::make_shared<Texture>();
                }
                else {
                    currentTextureVerticies[j] = m_textures[textureVerticies[3 * i + j]];
                }
            }

            currentSurfaceNormal = &surfaceNormals[3 * normalVerticies[3 * i]];

            m_triangles.push_back(std::make_shared<Triangle>(currentTriangleVerticies, currentTextureVerticies, currentSurfaceNormal, colour));
            count++;
        }

        int endIndex = static_cast<int>(m_triangles.size());
        int startIndex = endIndex - count;

        m_triangleObjectPoints[meshName] = TriangleObjectPoints(startIndex, endIndex);
    }

    void MeshHandler::addMesh(std::string meshName)
    {
        int start = m_triangleObjectPoints[meshName].startIndex;
        int end = m_triangleObjectPoints[meshName].endIndex;

        int length = end - start;

        std::vector<std::shared_ptr<Triangle>> triangles;
        triangles.reserve(length);

        for (int i = start; i < end; i++) {
            triangles.push_back(m_triangles[i]);
        }

        m_meshObjects.push_back(std::make_shared<Mesh>(triangles));
    }

    void MeshHandler::generateVerticies()
    {
        std::vector<float>* currentMeshVerticies;

        for (const auto& currentMesh : m_meshObjects) {
            currentMeshVerticies = currentMesh->getVerticies();

            m_completeVerticies.insert(
                m_completeVerticies.end(),
                currentMeshVerticies->begin(),
                currentMeshVerticies->end()
            );
        }
    };
}
