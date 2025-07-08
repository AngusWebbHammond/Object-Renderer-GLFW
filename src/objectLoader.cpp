#pragma once
#include "objectLoader.hpp"

namespace ObjectRenderer {

    ObjectLoader::ObjectLoader() {
        m_edgeLength = 0;
        m_verticeLength = 0;
        m_vertexNormalsLength = 0;

        loadObjectFromFile();
    }
    ObjectLoader::~ObjectLoader()
    {
    }

    float* ObjectLoader::getVertices()
    {
        return m_vertices;
    }

    unsigned int* ObjectLoader::getEdges()
    {
        return m_edges;
    }

    int ObjectLoader::getVerticesLength()
    {
        return m_verticeLength;
    }

    int ObjectLoader::getEdgesLength()
    {
        return m_edgeLength;
    }

    void ObjectLoader::loadObjectFromFile() {
        char* filePath = "E:/VS Code/3D Object Renderer/objects/monkey.obj";

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
                std::string current = "";
                bool slash = false;

                for (auto val : values)
                {
                    if (std::isblank(val))
                    {
                        m_edges[m_edgeLength] = std::stoi(current) - 1;
                        m_edgeLength++;
                        current = "";
                        slash = false;
                    }
                    else if (val == '/')
                    {
                        slash = true;
                    }
                    else if (!slash) {
                        current.push_back(val);
                    }
                    else {
                        continue;
                    }
                }

                m_edges[m_edgeLength] = std::stoi(current) - 1;
                m_edgeLength++;
            }
        }
        objectFile.close();
    }
}