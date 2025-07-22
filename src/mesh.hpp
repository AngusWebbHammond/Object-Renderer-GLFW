//TODO create the mesh class, that can be associated with an object
#include "globals.hpp"

namespace ObjectRenderer {
    struct Vertex {
        float position[3];
        float texture[3];
    };

    struct Triangle {
        Vertex verticies[3];
        float surfaceNormal[3];
        float triangleVerticies[27];
        bool isEmmisible;

        Triangle(float* positions, float* textures, float* surfaceNormals);
        Triangle(float* positions, float* textures, float* surfaceNormals, bool _isEmmisible);
        ~Triangle() = default;

        void generateVerticies();
        void init(float* positions, float* textures, float* surfaceNormals);
    };

    class Mesh {
    public:
        Mesh() = default;

        void addTriangle(Triangle* triangle);
        void generateVertices();
        float* getVerticies();

    private:
        Triangle* m_triangles[g_maxBufferSize];
        int m_triangleNumber;
        float m_verticies[g_maxBufferSize];
        int m_verticiesNumber;
    };
}
