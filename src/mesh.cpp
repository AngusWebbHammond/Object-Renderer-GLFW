#include "mesh.hpp"

namespace ObjectRenderer {
    Triangle::Triangle(float* positions, float* textures, float* surfaceNormals)
    {
        init(positions, textures, surfaceNormals);
        generateVerticies();
        isEmmisible = false;
    }

    Triangle::Triangle(float* positions, float* textures, float* surfaceNormals, bool _isEmmisible)
    {
        init(positions, textures, surfaceNormals);
        generateVerticies();
        isEmmisible = _isEmmisible;
    }


    void Triangle::generateVerticies()
    {
        int count = 0;
        for (auto point : verticies) {
            for (int i = 0; i < 3; i++) {
                triangleVerticies[3 * count + i] = point.position[i];
                triangleVerticies[3 * count + i * 2] = point.texture[i];
                triangleVerticies[3 * count + i * 3] = surfaceNormal[i];
            }
        }
    }

    void Triangle::init(float* positions, float* textures, float* surfaceNormals)
    {
        for (int i = 0; i < 3; i++) {
            surfaceNormal[i] = surfaceNormals[i];

            for (int j = 0; j < 3; j++) {
                verticies[i].position[j] = positions[j];
                verticies[i].texture[j] = textures[j];
            }
        }
    }

    void Mesh::addTriangle(Triangle* triangle)
    {
        m_triangles[m_triangleNumber] = triangle;
        m_triangleNumber++;
    }

    void Mesh::generateVertices()
    {
        for (int i = 0; i < m_triangleNumber; i++) {
            float* verticies[] = { m_triangles[i]->triangleVerticies };
            for (float* vert : verticies) {
                m_verticies[m_verticiesNumber] = *vert;
                m_verticiesNumber++;
            }
        }
    }
    float* Mesh::getVerticies()
    {
        return m_verticies;
    }
}