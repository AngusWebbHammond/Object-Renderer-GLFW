#pragma once

#include "../Core/globals.hpp"

#include <memory>
#include <vector>
#include <array>
#include <string>

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
        std::vector<float> m_texture;

        void setTexture(const std::vector<float>& texture);

    public:
        Texture();
        Texture(const std::vector<float>& texture);
        std::vector<float> getTexture();
    };

    struct Triangle {
    public:
        Triangle();
        Triangle(std::shared_ptr<Vertex>* verticies, std::shared_ptr<Texture>* textures, float* surfaceNormal, float* colour);
        ~Triangle() = default;

        std::array<float, 33> getTriangleVerticies() const;

    private:
        std::shared_ptr<Vertex> m_verticies[3];
        std::shared_ptr<Texture> m_textures[3];
        float m_surfaceNormal[3];
        float m_colour[3];
        std::array<float, 33> m_triangleVerticies;

        void generateVerticies();
    };

    class Mesh {
    public:
        Mesh();
        Mesh(const std::vector<std::shared_ptr<Triangle>>& triangles, std::string& meshName);

        std::vector<float>* getVerticies();
        std::string getMeshName();
        int getMeshTrianglesLength();

    private:
        std::vector<std::shared_ptr<Triangle>> m_triangles;
        std::vector<float> m_verticies;

        std::string m_meshName;

        void generateVertices();
    };
}
