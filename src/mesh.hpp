//TODO create the mesh class, that can be associated with an object
#pragma once
#include "globals.hpp"
#include <memory>

namespace ObjectRenderer {
    struct Vertex {
    private:
        float m_position[3];

    public:
        Vertex();
        Vertex(float* position);
        float* getPosition();
    };

    struct Texture {
    private:
        float m_texture[2];

    public:
        Texture();
        Texture(float* texture);
        float* getTexture();
    };

    struct Triangle {
    public:
        Triangle();
        Triangle(std::shared_ptr<Vertex>* verticies, std::shared_ptr<Texture>* textures, float* surfaceNormal, float* colour);
        ~Triangle() = default;

        float* getTriangleVerticies();

    private:
        std::shared_ptr<Vertex> m_verticies[3];
        std::shared_ptr<Texture> m_textures[3];
        float m_surfaceNormal[3];
        float m_colour[3];
        float m_triangleVerticies[33];

        void generateVerticies();
    };

    class Mesh {
    public:
        Mesh();
        Mesh(std::shared_ptr<Triangle>* triangles, int numberOfTriangles);

        float* getVerticies();
        int getVerticiesLength();

    private:
        std::shared_ptr<Triangle> m_triangles[g_maxBufferSize];
        int m_trianglesLength;

        float m_verticies[g_maxBufferSize];
        int m_verticiesLength;

        void generateVertices();
    };
}
