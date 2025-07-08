#pragma once

#ifndef GLAD_GL_IMPLEMENTATION
#define GLAD_GL_IMPLEMENTATION
#endif

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ObjectRenderer {

    class Shader {
    public:
        Shader();
        ~Shader();
        void init(const char* vertexPath, const char* fragmentPath);
        void use();

    private:
        void generateShaderLogs();
        void deleteShaders();
        void createShaderFromFile(const char* vertexPath, const char* fragmentPath);

    private:
        GLuint m_vertexShader;
        GLuint m_fragmentShader;
        GLuint m_shaderProgram;
        GLsizei m_logLengthVert, m_logLengthFrag;
        GLchar m_messageVert[1024], m_messageFrag[1024];
        glm::mat4 m_trans;
    };
}