#pragma once
#include "objectLoader.hpp"

namespace ObjectRenderer {

    ObjectLoader::ObjectLoader() {
        m_verticeLength = 0;
        m_vertexNormalsLength = 0;
        m_texturesLength = 0;

        m_edgeLength = 0;
        m_vertexNormalEdgeLength = 0;
        m_textureEdgeLength = 0;

        loadObjectFromFile();
    }
    ObjectLoader::~ObjectLoader()
    {
    }

    float* ObjectLoader::getVertices()
    {
        return m_vertices;
    }

    float* ObjectLoader::getTextures()
    {
        return m_textures;
    }

    float* ObjectLoader::getNormals()
    {
        return m_vertexNormals;
    }

    int* ObjectLoader::getEdges()
    {
        return m_edges;
    }

    int* ObjectLoader::getTextureEdges()
    {
        return m_textureEdges;
    }

    int* ObjectLoader::getNormalEdges()
    {
        return m_vertexNormalEdges;
    }

    int ObjectLoader::getVerticesLength()
    {
        return m_verticeLength;
    }

    int ObjectLoader::getTexturesLength()
    {
        return m_texturesLength;
    }

    int ObjectLoader::getNormalsLength()
    {
        return m_vertexNormalsLength;
    }

    int ObjectLoader::getEdgesLength()
    {
        return m_edgeLength;
    }

    int ObjectLoader::getTextureEdgesLength()
    {
        return m_textureEdgeLength;
    }

    int ObjectLoader::getNormalEdgesLength()
    {
        return m_vertexNormalEdgeLength;
    }

    void ObjectLoader::loadObjectFromFile() {
        char* filePath = "../../objects/sphere.obj";

        std::ifstream objectFile;
        std::string line;

        objectFile.open(filePath);

        while (!objectFile.eof())
        {
            std::getline(objectFile, line);
            if (line.length() == 0) {
                continue;
            }
            char start = line[0];
            char second = line[1];
            if (start == '#')
            {
                continue;
            }
            else if (start == 'v' && second != 't' && second != 'n') {
                std::string values = line.substr(2, line.length() - 2);
                std::string current = "";
                for (auto val : values) {
                    if (std::isblank(val)) {
                        m_vertices[m_verticeLength] = std::stof(current);
                        m_verticeLength++;
                        current = "";
                    }
                    else {
                        current.push_back(val);
                    }
                }
                m_vertices[m_verticeLength] = std::stof(current);
                m_verticeLength++;
            }
            else if (start == 'v' && second == 'n') {
                std::string values = line.substr(3, line.length() - 3);
                std::string current = "";
                for (auto val : values) {
                    if (std::isblank(val)) {
                        m_vertexNormals[m_vertexNormalsLength] = std::stof(current);
                        m_vertexNormalsLength++;
                        current = "";
                    }
                    else {
                        current.push_back(val);
                    }
                }
                m_vertexNormals[m_vertexNormalsLength] = std::stof(current);
                m_vertexNormalsLength++;
            }
            else if (start == 'f')
            {
                std::string values = line.substr(2, line.length() - 2);
                std::string currentEdge = "";
                std::string currentTexture = "";
                std::string currentNormal = "";
                int count = 0;

                for (auto val : values)
                {
                    if (std::isblank(val))
                    {
                        m_edges[m_edgeLength] = std::stoi(currentEdge) - 1;
                        m_edgeLength++;
                        currentEdge = "";

                        m_vertexNormalEdges[m_vertexNormalEdgeLength] = std::stoi(currentNormal) - 1;
                        m_vertexNormalEdgeLength++;
                        currentNormal = "";

                        m_textureEdges[m_textureEdgeLength] = std::stoi(currentTexture) - 1;
                        m_textureEdgeLength++;
                        currentTexture = "";

                        count = 0;
                    }
                    else if (val == '/')
                    {
                        count++;
                    }
                    else if (count == 0) {
                        currentEdge.push_back(val);
                    }
                    else if (count == 1) {
                        currentTexture.push_back(val);
                    }
                    else if (count == 2) {
                        currentNormal.push_back(val);
                    }
                    else {
                        continue;
                    }
                }

                m_edges[m_edgeLength] = std::stoi(currentEdge) - 1;
                m_edgeLength++;

                m_vertexNormalEdges[m_vertexNormalEdgeLength] = std::stoi(currentNormal) - 1;
                m_vertexNormalEdgeLength++;

                m_textureEdges[m_textureEdgeLength] = std::stoi(currentTexture) - 1;
                m_textureEdgeLength++;
            }
            else if (start == 'v' && second == 't') {
                std::string values = line.substr(3, line.length() - 3);
                std::string current = "";
                for (auto val : values) {
                    if (std::isblank(val)) {
                        m_textures[m_texturesLength] = std::stof(current);
                        m_texturesLength++;
                        current = "";
                    }
                    else {
                        current.push_back(val);
                    }
                }
                m_textures[m_texturesLength] = std::stof(current);
                m_texturesLength++;
            }
            //TODO Add more cases to this
        }
        objectFile.close();
    }
}