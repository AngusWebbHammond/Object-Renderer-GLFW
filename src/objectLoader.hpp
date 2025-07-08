#pragma once
#include <fstream>
#include <sstream>
#include <iostream>

namespace ObjectRenderer
{
    class ObjectLoader
    {
    public:
        ObjectLoader();
        ~ObjectLoader();

        float* getVertices();
        unsigned int* getEdges();
        int getVerticesLength();
        int getEdgesLength();

    private:
        void loadObjectFromFile();

        float m_vertices[10000];
        float m_vertexNormals[10000];
        unsigned int m_edges[10000];
        int m_verticeLength;
        int m_vertexNormalsLength;
        int m_edgeLength;
    };
} // namespace ObjectRenderer
