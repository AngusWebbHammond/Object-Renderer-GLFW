#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include "globals.hpp"

namespace ObjectRenderer
{
    class ObjectLoader
    {
    public:
        ObjectLoader();
        ~ObjectLoader();

        float* getVertices();
        float* getTextures();
        float* getNormals();

        int* getEdges();
        int* getTextureEdges();
        int* getNormalEdges();

        int getVerticesLength();
        int getTexturesLength();
        int getNormalsLength();

        int getEdgesLength();
        int getTextureEdgesLength();
        int getNormalEdgesLength();

    private:
        void loadObjectFromFile();

        float m_vertices[g_maxBufferSize];
        float m_vertexNormals[g_maxBufferSize];
        float m_textures[g_maxBufferSize];

        int m_edges[g_maxBufferSize];
        int m_textureEdges[g_maxBufferSize];
        int m_vertexNormalEdges[g_maxBufferSize];

        int m_verticeLength;
        int m_vertexNormalsLength;
        int m_texturesLength;

        int m_edgeLength;
        int m_vertexNormalEdgeLength;
        int m_textureEdgeLength;

    };
} // namespace ObjectRenderer
