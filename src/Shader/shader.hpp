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
#include <string>

namespace ObjectRenderer {

    class Shader {
    public:
        Shader();
        ~Shader();
        void init(const char* vertexPath, const char* fragmentPath);
        void use();
        GLuint getShaderProgram();

        void setMat4(const std::string& name, glm::mat4 value) const;
        void setVec3(const std::string& name, glm::vec3 value) const;
        void setFloat(const std::string& name, glm::float32 value) const;
        void setInt(const std::string& name, int value) const;
        void setBool(const std::string& name, bool value) const;

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
    };
}