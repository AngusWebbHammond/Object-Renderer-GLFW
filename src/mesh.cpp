#pragma once
#include "mesh.hpp"

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

    Texture::Texture() {
        for (int i = 0; i < 2; i++) {
            m_texture[i] = 0.0f;
        }
    }

    Texture::Texture(float* texture) {
        for (int i = 0; i < 2; i++) {
            m_texture[i] = texture[i];
        }
    }

    float* Texture::getTexture() {
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
        float* texture;

        for (int i = 0; i < 3; i++) {
            position = m_verticies[i]->getPosition();
            texture = m_textures[i]->getTexture();

            for (int j = 0; j < 3; j++) {
                m_triangleVerticies[11 * i + j] = position[j];
                m_triangleVerticies[11 * i + j + 3] = m_colour[j];
                m_triangleVerticies[11 * i + j + 6] = m_surfaceNormal[j];
                if (j != 2) {
                    m_triangleVerticies[11 * i + j + 9] = texture[j];
                }
            }
        }
    }

    float* Triangle::getTriangleVerticies()
    {
        return m_triangleVerticies;
    }
}

// Mesh Class
namespace ObjectRenderer {

    Mesh::Mesh() {}

    Mesh::Mesh(std::shared_ptr<Triangle>* triangles, int numberOfTriangles)
    {
        m_verticiesLength = 0;
        m_trianglesLength = 0;

        for (int i = 0; i < numberOfTriangles; i++) {
            m_triangles[i] = triangles[i];
        }

        m_trianglesLength = numberOfTriangles;

        generateVertices();
    }

    float* Mesh::getVerticies()
    {
        return m_verticies;
    }

    int Mesh::getVerticiesLength()
    {
        return m_verticiesLength;
    }

    void Mesh::generateVertices()
    {
        float* vertex;

        for (int i = 0; i < m_trianglesLength; i++) {
            vertex = m_triangles[i]->getTriangleVerticies();

            for (int j = 0; j < 33; j++) {
                m_verticies[33 * i + j] = vertex[j];
                m_verticiesLength++;
            }
        }
    }
}