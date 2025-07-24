#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "globals.hpp"

namespace ObjectRenderer
{
    class ObjectLoader
    {
    public:
        ObjectLoader();
        ~ObjectLoader();

        void loadObjectFromFile(char* filePath);

        std::vector<float> getVertices();
        std::vector<float> getTextures();
        std::vector<float> getNormals();

        std::vector<int> getEdges();
        std::vector<int> getTextureEdges();
        std::vector<int> getNormalEdges();

    private:


        std::vector<float> m_vertices;
        std::vector<float> m_vertexNormals;
        std::vector<float> m_textures;

        std::vector<int> m_edges;
        std::vector<int> m_textureEdges;
        std::vector<int> m_vertexNormalEdges;
    };
} // namespace ObjectRenderer
