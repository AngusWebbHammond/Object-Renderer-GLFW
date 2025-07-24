#pragma once
#include "objectLoader.hpp"

namespace ObjectRenderer {
    ObjectLoader::ObjectLoader()
    {
    }

    ObjectLoader::~ObjectLoader()
    {
    }

    std::vector<float> ObjectLoader::getVertices()
    {
        return m_vertices;
    }

    std::vector<float> ObjectLoader::getTextures()
    {
        return m_textures;
    }

    std::vector<float> ObjectLoader::getNormals()
    {
        return m_vertexNormals;
    }

    std::vector<int> ObjectLoader::getEdges()
    {
        return m_edges;
    }

    std::vector<int> ObjectLoader::getTextureEdges()
    {
        return m_textureEdges;
    }

    std::vector<int> ObjectLoader::getNormalEdges()
    {
        return m_vertexNormalEdges;
    }

    void ObjectLoader::loadObjectFromFile(char* filePath) {
        m_vertices.clear();
        m_textures.clear();
        m_vertexNormals.clear();

        m_edges.clear();
        m_textureEdges.clear();
        m_vertexNormalEdges.clear();

        std::ifstream objectFile(filePath);
        std::string line;

        if (!objectFile.is_open()) {
            std::cerr << "Failed to open file: " << filePath << std::endl;
            return;
        }

        while (std::getline(objectFile, line))
        {
            if (line.empty() || line[0] == '#') {
                continue;
            }

            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v") {
                float x, y, z;
                iss >> x >> y >> z;
                m_vertices.insert(m_vertices.end(), { x, y, z });
            }

            else if (prefix == "vn") {
                float nx, ny, nz;
                iss >> nx >> ny >> nz;
                m_vertexNormals.insert(m_vertexNormals.end(), { nx, ny, nz });
            }
            else if (prefix == "vt") {
                float u, v;
                iss >> u >> v;
                m_textures.insert(m_textures.end(), { u, v });
            }
            else if (prefix == "f") {
                std::string vertexStr;
                while (iss >> vertexStr) {
                    std::istringstream vertexStream(vertexStr);
                    std::string vIdx, tIdx, nIdx;

                    std::getline(vertexStream, vIdx, '/');
                    std::getline(vertexStream, tIdx, '/');
                    std::getline(vertexStream, nIdx, '/');

                    if (!vIdx.empty())
                        m_edges.push_back(std::stoi(vIdx) - 1);
                    if (!tIdx.empty())
                        m_textureEdges.push_back(std::stoi(tIdx) - 1);
                    if (!nIdx.empty())
                        m_vertexNormalEdges.push_back(std::stoi(nIdx) - 1);
                }
            }
        }

        objectFile.close();

    }
}