#pragma once
#include "mesh.hpp"
#include <vector>
#include <memory>
#include <array>

// Vertex Class
namespace ObjectRenderer {

    Vertex::Vertex() {
        for (int i = 0; i < 3; i++) {
            m_position[i] = 0.0f;
        }
    }

    Vertex::Vertex(float* position)
    {
        for (int i = 0; i < 3; i++) {
            m_position[i] = position[i];
        }
    }

    float* Vertex::getPosition()
    {
        return m_position;
    }
}

// Texture Class
namespace ObjectRenderer {
    void Texture::setTexture(const std::vector<float>& texture) {
        if (texture.size() >= 2) {
            m_texture[0] = texture[0];
            m_texture[1] = texture[1];
        }
        else {
            m_texture.assign(2, 0.0f);
        }
    }

    Texture::Texture() : m_texture(2, 0.0f) {}

    Texture::Texture(const std::vector<float>& texture) : m_texture(2, 0.0f) {
        setTexture(texture);
    }

    std::vector<float> Texture::getTexture()
    {
        return m_texture;
    }
}

// Triangle Class
namespace ObjectRenderer {

    Triangle::Triangle()
    {
        for (int i = 0; i < 3; i++) {
            m_verticies[i] = nullptr;
            m_surfaceNormal[i] = 0.0f;
            m_colour[i] = 0.0f;
        }

        for (int i = 0; i < 2; i++) {
            m_textures[i] = nullptr;
        }
    }

    Triangle::Triangle(std::shared_ptr<Vertex>* verticies, std::shared_ptr<Texture>* textures, float* surfaceNormal, float* colour)
    {
        for (int i = 0; i < 3; i++) {
            m_verticies[i] = verticies[i];
            m_surfaceNormal[i] = surfaceNormal[i];
            m_colour[i] = colour[i];
            m_textures[i] = textures[i];
        }

        generateVerticies();
    }

    void Triangle::generateVerticies()
    {
        float* position;
        std::vector<float> texture;

        for (int i = 0; i < 3; i++) {
            position = m_verticies[i]->getPosition();
            texture = m_textures[i]->getTexture();

            for (int j = 0; j < 3; j++) {
                m_triangleVerticies[11 * i + j] = position[j];
                m_triangleVerticies[11 * i + j + 3] = m_colour[j];
                m_triangleVerticies[11 * i + j + 6] = m_surfaceNormal[j];
            }

            for (int j = 0; j < 2; j++) {
                m_triangleVerticies[11 * i + j + 9] = texture[j];
            }
        }
    }

    std::array<float, 33> Triangle::getTriangleVerticies() const
    {
        return m_triangleVerticies;
    }
}

// Mesh Class
namespace ObjectRenderer {

    Mesh::Mesh() {}

    Mesh::Mesh(const std::vector<std::shared_ptr<Triangle>>& triangles)
    {
        m_triangles = triangles;
        m_verticies.reserve(static_cast<int>(m_triangles.size()) * 33);
        generateVertices();
    }

    std::vector<float>* Mesh::getVerticies()
    {
        return &m_verticies;
    }

    void Mesh::generateVertices()
    {
        for (const auto& triangle : m_triangles) {
            const std::array<float, 33>& vertexData = triangle->getTriangleVerticies();
            m_verticies.insert(m_verticies.end(), vertexData.begin(), vertexData.end());
        }
    }
}