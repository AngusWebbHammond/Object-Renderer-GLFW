#pragma once

#include "meshHandler.hpp"

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
        m_previousVerticiesSize = 0;
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

    void MeshHandler::addObjectFromFile(const char* filePath)
    {
        m_objectLoader.loadObjectFromFile(filePath);

        std::vector<float> vertices = m_objectLoader.getVertices();
        std::vector<float> normals = m_objectLoader.getNormals();
        std::vector<float> textures = m_objectLoader.getTextures();

        std::vector<int> edges = m_objectLoader.getEdges();
        std::vector<int> textureEdges = m_objectLoader.getTextureEdges();
        std::vector<int> normalEdges = m_objectLoader.getNormalEdges();

        std::string name = m_objectLoader.getName();

        float colour[] = { 1.0f, 0.0f, 1.0f };

        addObject(vertices, textures, edges, colour, normals, normalEdges, textureEdges, name);

        createAndRegisterMeshVAO(name);
    }

    std::vector<float> MeshHandler::getVerticies()
    {
        return m_completeVerticies;
    }

    TriangleObjectPoints MeshHandler::getMeshIndexStartEnd(std::string name)
    {
        return m_triangleObjectPoints[name];
    }

    void MeshHandler::createAndRegisterMeshVAO(const std::string& meshName)
    {
        GLuint vao;
        glGenVertexArrays(1, &vao);
        m_vaos[meshName] = vao;
    }

    void MeshHandler::bindMeshVAO(const std::string& meshName)
    {
        auto it = m_vaos.find(meshName);
        if (it != m_vaos.end()) {
            glBindVertexArray(it->second);
        }
        else {
            std::cerr << "Warning: VAO for mesh '" << meshName << "' not found." << std::endl;
            glBindVertexArray(0); // Unbind VAO to prevent bad draw calls
        }
    }

    std::vector<std::string> MeshHandler::getVAONames()
    {
        std::vector<std::string> tempVectorStrings;
        for (auto it : m_vaos) {
            tempVectorStrings.push_back(it.first);
        }
        return tempVectorStrings;
    }

    TriangleObjectPoints MeshHandler::getDrawArrayStartEndPoints(std::string meshName)
    {
        return m_drawArraysStartEndPoints[meshName];
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
                currentTriangleVerticies[j] = m_verticies[triangleVerticies[3 * i + j] + m_previousVerticiesSize];
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
        m_previousVerticiesSize = static_cast<int>(m_verticies.size());
    }

    void MeshHandler::addMesh(std::string meshName)
    {
        int start = m_triangleObjectPoints[meshName].startIndex;
        int end = m_triangleObjectPoints[meshName].endIndex;

        int length = end - start;

        std::vector<std::shared_ptr<Triangle>> triangles;
        triangles.reserve(length);

        for (int i = 0; i < length; i++) {
            triangles.push_back(m_triangles[i + start]);
        }

        m_meshObjects.push_back(std::make_shared<Mesh>(triangles, meshName));
    }

    void MeshHandler::generateVerticies()
    {
        m_completeVerticies.clear();
        std::vector<float>* currentMeshVerticies;
        int count = 0;

        for (const auto& currentMesh : m_meshObjects) {
            currentMeshVerticies = currentMesh->getVerticies();
            std::string currentMeshName = currentMesh->getMeshName();

            m_completeVerticies.insert(
                m_completeVerticies.end(),
                currentMeshVerticies->begin(),
                currentMeshVerticies->end()
            );

            m_drawArraysStartEndPoints[currentMeshName].startIndex = count;
            m_drawArraysStartEndPoints[currentMeshName].endIndex = count + currentMesh->getMeshTrianglesLength();
            count += currentMesh->getMeshTrianglesLength();
        }
    };
}
